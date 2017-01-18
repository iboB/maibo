// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// the main window class

#if defined(_MSC_VER) || defined(MAIBO_PLATFORM_MOBILE)
#   include <SDL_syswm.h>
#else
#   include <SDL2/SDL_syswm.h>
#endif

#include "Rendering/RenderTarget.h"

namespace maibo
{
    class MainWindow : public RenderTarget
    {
    public:
        struct CreationParameters
        {
            CreationParameters();

            const char* title;
            yama::uvector2 clientAreaSize;
            bool isFullScreen;
        };

        ~MainWindow();

        bool create(const CreationParameters& params);

        void setTitle(const char* title);

        const yama::uvector2& clientAreaSize() const { return m_creationParameters.clientAreaSize; }

        void swapBuffers();

        const SDL_SysWMinfo& sdlSysWMInfo() const { return m_sdlSysWMInfo; }

        // RenderTarget methods
        virtual const yama::uvector2& surfaseSize() const override { return clientAreaSize(); }
        virtual void activate() override;

    protected:
        SDL_Window* m_sdlWindow = nullptr;
        SDL_GLContext m_glContext;

        SDL_SysWMinfo m_sdlSysWMInfo;

        CreationParameters m_creationParameters;
    };
}
