// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/GUI/LibRocket/LibRocketRenderInterface.h>

#include <maibo/Shader.h>
#include <maibo/GPUProgram.h>
#include <maibo/Texture.h>
#include <maibo/lib/GLSentries.h>

#include <Rocket/Core/Context.h>

using namespace maibo;
using namespace std;
using namespace mathgp;

namespace
{
    const char* vertexShaderSource =
        "\
        uniform mat4 proj; \
        uniform vec2 translation; \
        attribute vec2 v_pos; \
        attribute vec2 v_texCoord; \
        attribute vec4 v_color; \
        varying vec2 texCoord; \
        varying vec4 color; \
        void main(void) \
        { \
            vec2 pos = v_pos + translation; \
            gl_Position = proj * vec4(pos.xy, 0.0, 1.0); \
            texCoord = v_texCoord; \
            color = v_color; \
        } \
        ";

    const char* fragmentTextureShaderSource =
        "\
        #version 100 \n\
        precision mediump float; \
        uniform sampler2D tex; \
        varying vec2 texCoord; \
        varying vec4 color; \
        void main(void) \
        { \
            gl_FragColor = color * texture2D(tex, texCoord); \
        } \
        ";

    const char* fragmentColorShaderSource =
        "\
        #version 100 \n\
        precision mediump float; \
        varying vec4 color; \
        void main(void) \
        { \
            gl_FragColor = color; \
        } \
        ";

    int Attrib_Position, Attrib_TexCoord, Attrib_Color;
}

LibRocketRenderInterface::DrawBuffers::DrawBuffers()
{
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);
}

LibRocketRenderInterface::DrawBuffers::~DrawBuffers()
{
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}


LibRocketRenderInterface::LibRocketRenderInterface()
{
    auto texFS = make_shared<Shader>(ShaderType::Fragment, "rocket texture fragment shader");
    texFS->load(fragmentTextureShaderSource);

    auto colorFS = make_shared<Shader>(ShaderType::Fragment, "rocket color fragment shader");
    colorFS->load(fragmentColorShaderSource);

    auto vs = make_shared<Shader>(ShaderType::Vertex, "rocket vertex shader");
    vs->load(vertexShaderSource);

    /////////////////////////////////////
    m_colorProgram = make_shared<GPUProgram>("rocket color program");

    m_colorProgram->attachShader(vs);
    m_colorProgram->attachShader(colorFS);

    Attrib_Position = m_colorProgram->bindCustomAttribute("v_pos");
    Attrib_Color = m_colorProgram->bindCustomAttribute("v_color");
    Attrib_TexCoord = -1;

    m_colorProgram->link();

    m_colorProjParam = m_colorProgram->getParameterByName("proj");
    m_colorTranslationParam = m_colorProgram->getParameterByName("translation");

    /////////////////////////////////////
    m_textureProgram = make_shared<GPUProgram>("rocket texture program");

    m_textureProgram->attachShader(vs);
    m_textureProgram->attachShader(texFS);

    Attrib_Position = m_textureProgram->bindCustomAttribute("v_pos");
    Attrib_TexCoord = m_textureProgram->bindCustomAttribute("v_texCoord");
    Attrib_Color = m_textureProgram->bindCustomAttribute("v_color");

    m_textureProgram->link();

    m_textureParam = m_textureProgram->getParameterByName("tex");
    m_textureProjParam = m_textureProgram->getParameterByName("proj");
    m_textureTranslationParam = m_textureProgram->getParameterByName("translation");

    /////////////////////////////////////
}

LibRocketRenderInterface::~LibRocketRenderInterface()
{

}

void LibRocketRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
    int* indices, int numIndices, Rocket::Core::TextureHandle textureHandle,
    const Rocket::Core::Vector2f& translation)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_immediateDrawBuffers.vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_immediateDrawBuffers.indexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Rocket::Core::Vertex) * numVertices, vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numIndices, indices, GL_DYNAMIC_DRAW);

    auto size = GetContext()->GetDimensions();

    // custom ortho 2d matrix
    const matrix projection = matrix::rows(
        2.f/size.x,   0,             0,  -1,
        0,           -2.f/size.y,    0,   1,
        0,            0,             0,   0,
        0,            0,             0,   1
        );
    // matrix::ortho_rh(0, w, h, 0, 0, 1); // note the inverted height. LibRocket uses 0,0 as top left

    if (textureHandle)
    {
        m_textureProgram->use();
        m_textureProgram->setParameter(m_textureProjParam, projection);
        m_textureProgram->setParameter(m_textureTranslationParam, vc(translation.x, translation.y));
    }
    else
    {
        m_colorProgram->use();
        m_colorProgram->setParameter(m_colorProjParam, projection);
        m_colorProgram->setParameter(m_colorTranslationParam, vc(translation.x, translation.y));
    }

    MAIBO_GL_SENTRY(GLEnableAttrib, Attrib_Position);
    MAIBO_GL_SENTRY(GLEnableAttrib, Attrib_Color);

    glVertexAttribPointer(Attrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), OFFSET_OF(Rocket::Core::Vertex, position));
    glVertexAttribPointer(Attrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Rocket::Core::Vertex), OFFSET_OF(Rocket::Core::Vertex, colour));

    if (textureHandle)
    {
        auto tex = reinterpret_cast<Texture*>(textureHandle);
        m_textureProgram->setParameter(m_textureParam, *tex);
        glVertexAttribPointer(Attrib_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), OFFSET_OF(Rocket::Core::Vertex, tex_coord));
        glEnableVertexAttribArray(Attrib_TexCoord);
    }

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    if (textureHandle)
    {
        glDisableVertexAttribArray(Attrib_TexCoord);
    }
}


Rocket::Core::CompiledGeometryHandle LibRocketRenderInterface::CompileGeometry(Rocket::Core::Vertex*, int, int*, int, Rocket::Core::TextureHandle)
{
    // no obvious need to support this yet
    return 0;
}
void LibRocketRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle, const Rocket::Core::Vector2f&) {}
void LibRocketRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle) {}

void LibRocketRenderInterface::EnableScissorRegion(bool enable)
{
    if (enable)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

void LibRocketRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
    glScissor(x, -y, width, height);
}

bool LibRocketRenderInterface::LoadTexture(Rocket::Core::TextureHandle& outTextureHandle,
    Rocket::Core::Vector2i& outTextureDimensions, const Rocket::Core::String& resourcePath)
{
    auto tex = new Texture(resourcePath.CString());

    tex->loadFromFile(resourcePath.CString());

    outTextureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(tex);

    return !!tex;
}

bool LibRocketRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& outTextureHandle, const Rocket::Core::byte* data,
    const Rocket::Core::Vector2i& sourceDimensions)
{
    auto tex = new Texture("libRocket generated");
    tex->loadFromData(GL_RGBA, sourceDimensions.x, sourceDimensions.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

    outTextureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(tex);

    return !!tex;
}

void LibRocketRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle textureHandle)
{
    auto tex = reinterpret_cast<Texture*>(textureHandle);
    delete tex;
}

void LibRocketRenderInterface::Release()
{
    delete this;
}