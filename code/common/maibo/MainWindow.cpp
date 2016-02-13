// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

// the main window class definition
#include "MainWindow.h"
#include "Rendering/RenderTargetManager.h"

using namespace std;
using namespace maibo;

MainWindow::CreationParameters::CreationParameters()
    : title("Main Window")
    , clientAreaSize(mathgp::v(1024u, 768u))
    , isFullScreen(false)
{
}

bool MainWindow::create(const MainWindow::CreationParameters& params)
{
    m_creationParameters = params;

#if MAIBO_PLATFORM_MOBILE
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
    {
        m_creationParameters.clientAreaSize.x() = unsigned(displayMode.w);
        m_creationParameters.clientAreaSize.y() = unsigned(displayMode.h);
    }
#endif

    const auto& size = m_creationParameters.clientAreaSize;

    int fullScreen = m_creationParameters.isFullScreen ? SDL_WINDOW_FULLSCREEN : 0;

    m_sdlWindow = SDL_CreateWindow(
        m_creationParameters.title,
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

    RenderTargetManager::instance().setScreenRenderTarget(this);
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

// render target methods

void MainWindow::activate()
{
    RenderTarget::activate();
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#   error "IOS's main framebuffer is not 0"
#endif
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // no MRTs support yet, so no need to take care of them
    // but if we have MRTs, we should uncomment those lines
//#if defined(__ANDROID__) || TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
//#   error "OpenGL ES doesn't support MRTs"
//#endif
    //GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    //glDrawBuffers(1, drawBuffers);
}
