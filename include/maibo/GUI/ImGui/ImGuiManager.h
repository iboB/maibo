// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/GPUProgramPtr.h>
#include <maibo/TexturePtr.h>
#include <maibo/Manager.h>

struct ImDrawData;

// ImGui manager
// used to integrate ImGui (https://github.com/ocornut/imgui) into MaiBo

namespace maibo
{
    class ImGuiManager : public Manager
    {
    public:
        ImGuiManager();
        ~ImGuiManager();

        static ImGuiManager& instance();

    private:
        virtual bool initialize() override;
        virtual void deinitialize() override;

        virtual const char* name() const override;

        virtual void update(uint32_t dt) override;

        virtual bool handleEvent(const SDL_Event& event) override;

        virtual void endFrame() override;
        void render(); // called in endFrame in order to be last

        ///////////////////////////////////////
        // input

        // in order to work correctly if a button was pressed and then released before the end of a frame
        // we store the presses in this array
        std::array<bool, 5> m_mouseButtonState;

        ///////////////////////////////////////
        // rendering
        GPUProgramPtr m_gpuProgram;

        // uniforms
        int m_textureParam;
        int m_projParam;

        TexturePtr m_fontsTexture;

        friend void Render(ImDrawData* data);
    };
}

