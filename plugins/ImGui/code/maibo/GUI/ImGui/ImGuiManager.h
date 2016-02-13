// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "maibo/Rendering/GPUProgramPtr.h"
#include "maibo/Rendering/TexturePtr.h"
#include "maibo/Input/InputEventHandler.h"
#include "maibo/Common/ExplicitSingleton.h"
#include "maibo/Common/Signals/AutoSlot.h"

struct ImDrawData;

// ImGui manager
// used to integrate ImGui (https://github.com/ocornut/imgui) into MaiBo

namespace maibo
{
    class ImGuiManager : public ExplicitSingleton<ImGuiManager>, public AutoSlot, public InputEventHandler
    {
        MAIBO_DECLARE_EXPLICIT_SINGLETON(ImGuiManager);

        slot update(uint32_t dt);

        slot render(); // called from OnPostRender

        // from imput event handler
        virtual bool handleEvent(const SDL_Event& event) override;

        ///////////////////////////////////////
        // settings

        // we're drawing the ImGui cursor when possible
        // so preserve the sdl cursor state between update calls
        int m_oldSDLCursorState = SDL_ENABLE;

        ///////////////////////////////////////
        // input

        // in order to work correctly if a button was pressed and then released before the end of a frame
        // we store the presses in this array
        std::array<bool, 5> m_mouseButtonState;

        ///////////////////////////////////////
        // rendering
        GPUProgramPtr m_gpuProgram;

        GLuint m_vertexBuffer = 0;
        GLuint m_indexBuffer = 0;

        // uniforms
        int m_textureParam = -1;
        int m_projParam = -1;

        TexturePtr m_fontsTexture;

        static void imguiRenderCallback(ImDrawData* data);
    };
}

