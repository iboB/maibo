// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/GUI/ImGui/ImGuiManager.h>
#include <maibo/lib/GLSentries.h>
#include <maibo/Application.h>
#if defined(_WIN32)
// required only to get the hwnd
#   include <maibo/MainWindow.h>
#endif
#include <maibo/GPUProgram.h>
#include <maibo/Texture.h>
#include <maibo/Shader.h>
#include <maibo/RenderTargetManager.h>

#include <imgui.h>

using namespace std;
using namespace mathgp;
using namespace maibo;

namespace
{

    void SetClipboardText(const char* text)
    {
        SDL_SetClipboardText(text);
    }

    const char* GetClipboardText()
    {
        return SDL_GetClipboardText();
    }

    const char* vertexShaderSource =
        "\
        uniform mat4 proj; \
        attribute vec2 v_pos; \
        attribute vec2 v_texCoord; \
        attribute vec4 v_color; \
        varying vec2 texCoord; \
        varying vec4 color; \
        void main(void) \
        { \
            texCoord = v_texCoord; \
            color = v_color; \
            gl_Position = proj * vec4(v_pos.xy, 0.0, 1.0); \
        } \
        ";

    const char* fragmentShaderSource =
        "\
        #version 100 \n\
        precision mediump float; \
        uniform sampler2D tex; \
        varying vec2 texCoord; \
        varying vec4 color; \
        void main() \
        { \
            gl_FragColor = color * texture2D(tex, texCoord); \
        } \
        ";
}

namespace
{
    int Attrib_Position, Attrib_TexCoord, Attrib_Color;
}

void ImGuiManager::imguiRenderCallback(ImDrawData* data)
{
    if (data->CmdListsCount == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    auto& gui = ImGuiManager::instance();

    MAIBO_GL_SENTRY(GLEnable, GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    MAIBO_GL_SENTRY(GLDisable, GL_CULL_FACE);
    MAIBO_GL_SENTRY(GLDisable, GL_DEPTH_TEST);
    MAIBO_GL_SENTRY(GLEnable, GL_SCISSOR_TEST);

    // custom ortho 2d matrix
    const float w = ImGui::GetIO().DisplaySize.x;
    const float h = ImGui::GetIO().DisplaySize.y;
    //auto projection = matrix::ortho_rh(0, w, h, 0, 0, 1); // note the inverted height. ImGui uses 0,0 as top left
    const float ortho_projection[4][4] =
    {
        { 2.0f / w, 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / -h, 0.0f, 0.0f },
        { 0.0f, 0.0f, -1.0f, 0.0f },
        { -1.0f, 1.0f, 0.0f, 1.0f },
    };

    auto projection = matrix::from_ptr(*ortho_projection);

    gui.m_gpuProgram->use();
    gui.m_gpuProgram->setParameter(gui.m_projParam, projection);

    MAIBO_GL_SENTRY(GLEnableAttrib, Attrib_Position);
    MAIBO_GL_SENTRY(GLEnableAttrib, Attrib_TexCoord);
    MAIBO_GL_SENTRY(GLEnableAttrib, Attrib_Color);

    glBindBuffer(GL_ARRAY_BUFFER, gui.m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.m_indexBuffer);

    glVertexAttribPointer(Attrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), OFFSET_OF(ImDrawVert, pos));
    glVertexAttribPointer(Attrib_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), OFFSET_OF(ImDrawVert, uv));
    glVertexAttribPointer(Attrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), OFFSET_OF(ImDrawVert, col));

    for (int i = 0; i < data->CmdListsCount; ++i)
    {
        const auto list = data->CmdLists[i];

        glBufferData(GL_ARRAY_BUFFER, sizeof(ImDrawVert) * list->VtxBuffer.size(), list->VtxBuffer.Data, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ImDrawIdx) * list->IdxBuffer.size(), list->IdxBuffer.Data, GL_DYNAMIC_DRAW);

        ImDrawIdx* offsetPtr = nullptr;
        for (const auto& cmd : list->CmdBuffer)
        {
            if (cmd.UserCallback)
            {
                cmd.UserCallback(list, &cmd);
            }
            else
            {
                // draw
                const Texture* t = reinterpret_cast<Texture*>(cmd.TextureId);
                assert(t);
                gui.m_gpuProgram->setParameter(gui.m_textureParam, *t);
                glScissor(int(cmd.ClipRect.x), (int)(h - cmd.ClipRect.w), int(cmd.ClipRect.z - cmd.ClipRect.x), int(cmd.ClipRect.w - cmd.ClipRect.y));
                glDrawElements(GL_TRIANGLES, cmd.ElemCount, GL_UNSIGNED_SHORT, offsetPtr);
                gui.m_gpuProgram->resetUniforms();
            }
            offsetPtr += cmd.ElemCount;
        }
    }
}

ImGuiManager::ImGuiManager()
{
    m_mouseButtonState = {};
    auto& io = ImGui::GetIO();

    // callbacks
    io.RenderDrawListsFn = &ImGuiManager::imguiRenderCallback;
    io.GetClipboardTextFn = GetClipboardText;
    io.SetClipboardTextFn = SetClipboardText;

    // other config
    io.IniFilename = nullptr;

    auto& app = Application_Instance();
#if defined(_WIN32)
    // get hwnd for the hardware cursor
    HWND wnd = app.mainWindow().sdlSysWMInfo().info.win.window;
    io.ImeWindowHandle = wnd;
#endif

    // initialize buffers, textures and shaders

    auto vs = make_shared<Shader>(ShaderType::Vertex, "gui vertex shader");
    vs->load(vertexShaderSource);

    auto fs = make_shared<Shader>(ShaderType::Fragment, "gui fragment shader");
    fs->load(fragmentShaderSource);

    m_gpuProgram = make_shared<GPUProgram>("gui program");

    m_gpuProgram->attachShader(vs);
    m_gpuProgram->attachShader(fs);

    Attrib_Position = m_gpuProgram->bindCustomAttribute("v_pos");
    Attrib_TexCoord = m_gpuProgram->bindCustomAttribute("v_texCoord");
    Attrib_Color = m_gpuProgram->bindCustomAttribute("v_color");

    m_gpuProgram->link();

    m_projParam = m_gpuProgram->getParameterByName("proj");
    m_textureParam = m_gpuProgram->getParameterByName("tex");

    // fonts texture
    m_fontsTexture = make_shared<Texture>("gui fonts texture");

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    m_fontsTexture->loadFromData(GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    io.Fonts->TexID = m_fontsTexture.get();

    io.Fonts->ClearInputData();
    io.Fonts->ClearTexData();

    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);

    app.OnPreUpdate.connect<ImGuiManager, &ImGuiManager::update>(this);
    app.OnPostRender.connect<ImGuiManager, &ImGuiManager::render>(this);
    app.addGlobalInputEventHandler(this);
}

ImGuiManager::~ImGuiManager()
{
    auto& app = Application_Instance();
    app.removeGlobalInputEventHandler(this);

    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
    ImGui::GetIO().Fonts->TexID = nullptr;
    ImGui::Shutdown();
}

void ImGuiManager::update(uint32_t dt)
{
    auto& io = ImGui::GetIO();

    auto screenSize = RenderTargetManager::instance().currentRenderTargetSize();
    io.DisplaySize = ImVec2(float(screenSize.x()), float(screenSize.y()));
    io.DeltaTime = float(dt) / 1000;

    // mouse info
    int x, y;
    Uint8 buttons = SDL_GetMouseState(&x, &y);

    io.MousePos = ImVec2(float(x), float(y));

    // mouse button trick
    for (size_t i = 0; i < m_mouseButtonState.size(); ++i)
    {
        io.MouseDown[i] = (buttons & SDL_BUTTON(i + 1)) || m_mouseButtonState[i];
        m_mouseButtonState[i] = false;
    }

    // perepare keyboard modifiers
    io.KeyCtrl = io.KeysDown[SDL_SCANCODE_LCTRL] || io.KeysDown[SDL_SCANCODE_RCTRL];
    io.KeyAlt = io.KeysDown[SDL_SCANCODE_LALT] || io.KeysDown[SDL_SCANCODE_RALT];
    io.KeyShift = io.KeysDown[SDL_SCANCODE_LSHIFT] || io.KeysDown[SDL_SCANCODE_RSHIFT];

    // sdl mouse 1 is middle, imgui mouse 1 is right
    swap(io.MouseDown[1], io.MouseDown[2]);

    bool hadMouseFocur = io.WantCaptureMouse;
    bool wasInputtingText = io.WantInputCharacters;

    ImGui::NewFrame();

    if (hadMouseFocur && !io.WantCaptureMouse)
    {
        SDL_ShowCursor(m_oldSDLCursorState);
        io.MouseDrawCursor = false;
    }
    else if (!hadMouseFocur && io.WantCaptureMouse)
    {
        m_oldSDLCursorState = SDL_ShowCursor(SDL_QUERY);
        SDL_ShowCursor(SDL_DISABLE);
        io.MouseDrawCursor = true;
    }

    if (wasInputtingText && !io.WantInputCharacters)
    {
        SDL_StopTextInput();
    }
    else if (!wasInputtingText && io.WantInputCharacters)
    {
        SDL_StartTextInput();
    }

    io.MouseWheel = 0;
}

void ImGuiManager::render()
{
    ImGui::Render();
}

bool ImGuiManager::handleEvent(const SDL_Event& event)
{
    enum EventType
    {
        Other,
        Mouse,
        Keyboard,
    };

    EventType thisEventType = Other;

    auto& io = ImGui::GetIO();

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button - 1 < int(m_mouseButtonState.size()))
        {
            m_mouseButtonState[event.button.button - 1] = true;
        }
        thisEventType = Mouse;
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        thisEventType = Mouse;
    }
    else if (event.type == SDL_MOUSEWHEEL)
    {
        float wheelY = float(event.wheel.y) / 5;

#if defined(__EMSCRIPTEN__)
        // curiously the browser sends us percent wheel values in sdl
        wheelY /= 100;
#endif

        // SDL 2.0.4
        //if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
        //{
        //    wheelY *= -1;
        //}

        io.MouseWheel += wheelY;
        thisEventType = Mouse;
    }
    else if (event.type == SDL_KEYDOWN)
    {
        io.KeysDown[event.key.keysym.scancode] = true;
        thisEventType = Keyboard;
    }
    else if (event.type == SDL_KEYUP)
    {
        io.KeysDown[event.key.keysym.scancode] = false;
        thisEventType = Keyboard;
    }
    else if (event.type == SDL_TEXTINPUT)
    {
        io.AddInputCharactersUTF8(event.text.text);
        thisEventType = Keyboard;
    }


    switch (thisEventType)
    {
    case Mouse:
        return io.WantCaptureMouse;
    case Keyboard:
        return io.WantCaptureKeyboard;
    case Other: // preveting clang warnings
        return false;
    }

    // don't eat other events
    return false;
}

void ImGuiManager::endFrame()
{
    render();
}
