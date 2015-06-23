// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

// the main window class definition
#include <maibo/MainWindow.h>

using namespace std;
using namespace maibo;

MainWindow::CreationParameters::CreationParameters()
    : clientAreaSize(mathgp::v(1024u, 768u))
    , title("Main Window")
    , isFullScreen(false)
{
}

bool MainWindow::create(const MainWindow::CreationParameters& params)
{
    m_creationParameters = params;

    const auto& size = params.clientAreaSize;

    int fullScreen = params.isFullScreen ? SDL_WINDOW_FULLSCREEN : 0;

    m_sdlWindow = SDL_CreateWindow(
        params.title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        size.x(), size.y(),
        SDL_WINDOW_OPENGL|fullScreen);

    if(!m_sdlWindow)
    {
        cerr << "Can't create main window" << endl;
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_sdlWindow);

    if(!m_sdlWindow)
    {
        cerr << "Can't create GL context" << endl;
        return false;
    }

    SDL_VERSION(&m_sdlSysWMInfo.version);
    SDL_GetWindowWMInfo(m_sdlWindow, &m_sdlSysWMInfo);

    return true;
}

MainWindow::~MainWindow()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_sdlWindow);
}


void MainWindow::setTitle(const char* title)
{
    SDL_SetWindowTitle(m_sdlWindow, title);
}

void MainWindow::swapBuffers()
{
    SDL_GL_SwapWindow(m_sdlWindow);
}
