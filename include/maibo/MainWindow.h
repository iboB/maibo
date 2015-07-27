// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// the main window class

#if defined(_MSC_VER)
#   include <SDL_syswm.h>
#else
#   include <SDL2/SDL_syswm.h>
#endif

namespace maibo
{
    class MainWindow
    {
    public:
        struct CreationParameters
        {
            CreationParameters();

            const char* title;
            mathgp::uvector2 clientAreaSize;
            bool isFullScreen;
        };

        ~MainWindow();

        bool create(const CreationParameters& params);

        void setTitle(const char* title);

        const mathgp::uvector2& clientAreaSize() const { return m_creationParameters.clientAreaSize; }

        void swapBuffers();

        const SDL_SysWMinfo& sdlSysWMInfo() const { return m_sdlSysWMInfo; }

    protected:
        SDL_Window* m_sdlWindow = nullptr;
        SDL_GLContext m_glContext;

        SDL_SysWMinfo m_sdlSysWMInfo;

        CreationParameters m_creationParameters;
    };
}
