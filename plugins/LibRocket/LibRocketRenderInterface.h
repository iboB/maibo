// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/GPUProgramPtr.h>

#include <Rocket/Core/RenderInterface.h>

namespace maibo
{
    class Texture;

    class LibRocketRenderInterface : public Rocket::Core::RenderInterface
    {
    public:
        LibRocketRenderInterface();
        ~LibRocketRenderInterface();

        /// Called by Rocket when it wants to render geometry that it does not wish to optimise.
        virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
            int* indices, int numIndices, Rocket::Core::TextureHandle texture,
            const Rocket::Core::Vector2f& translation) override;

        /// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
        virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int numVertices,
            int* indices, int numIndices, Rocket::Core::TextureHandle texture) override;

        /// Called by Rocket when it wants to render application-compiled geometry.
        virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry,
            const Rocket::Core::Vector2f& translation) override;

        /// Called by Rocket when it wants to release application-compiled geometry.
        virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry) override;

        /// Called by Rocket when it wants to enable or disable scissoring to clip content.
        virtual void EnableScissorRegion(bool enable) override;
        /// Called by Rocket when it wants to change the scissor region.
        virtual void SetScissorRegion(int x, int y, int width, int height) override;

        /// Called by Rocket when a texture is required by the library.
        virtual bool LoadTexture(Rocket::Core::TextureHandle& outTextureHandle,
            Rocket::Core::Vector2i& outTextureDimensions, const Rocket::Core::String& resourcePath) override;
        /// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
        virtual bool GenerateTexture(Rocket::Core::TextureHandle& outTextureHandle, const Rocket::Core::byte* data,
            const Rocket::Core::Vector2i& sourceDimensions) override;
        /// Called by Rocket when a loaded texture is no longer required.
        virtual void ReleaseTexture(Rocket::Core::TextureHandle textureHandle) override;

        virtual void Release() override;

    private:
        GPUProgramPtr m_colorProgram;
        int m_colorProjParam, m_colorTranslationParam;

        GPUProgramPtr m_textureProgram;
        int m_textureParam, m_textureProjParam, m_textureTranslationParam;

        struct DrawBuffers
        {
            DrawBuffers();
            ~DrawBuffers();

            GLuint vertexBuffer;
            GLuint indexBuffer;
        };

        DrawBuffers m_immediateDrawBuffers;

        struct CompiledGeometry
        {
            DrawBuffers buf;
            Texture* texture;
            int numIndices;
        };

        void Render(int numIndices, const Texture* texture, const Rocket::Core::Vector2f& translation);
    };
}
