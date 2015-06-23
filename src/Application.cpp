// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/Application.h>

#include <maibo/TaskManager.h>
#include <maibo/lib/high_res_clock.h>
#include <maibo/MainWindow.h>

#if !defined(__EMSCRIPTEN__)
#   include <thread>
#endif

using namespace maibo;
using namespace std;

Application::Application()
{
}

Application::~Application()
{
}

bool Application::initialize(const Application::CreationParameters& cp)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        cerr << "Couldn't init sdl" << endl;
        return false;
    }

    MainWindow::CreationParameters mwc;
    mwc.clientAreaSize = cp.mainWindowClientAreaSize;
    mwc.isFullScreen = cp.isFullScreen;
    mwc.title = cp.mainWindowTitle;
    m_mainWindow = new MainWindow;
    if (!m_mainWindow->create(mwc))
    {
        return false;
    }

#if defined(_WIN32)
    if (glewInit() != GLEW_OK)
    {
        cerr << "couldn't init glew" << endl;
    }
#endif

    TaskManager::instance().setNumTasksPerUpdate(cp.numTasksPerUpdate);

    return true;
}

void Application::deinitialize()
{
    delete m_mainWindow;
    m_mainWindow = nullptr;
    SDL_Quit();
}

void Application::beginFrame()
{

}

void Application::handleInput()
{

}

void Application::update()
{
    TaskManager::instance().update();
}

void Application::render()
{

}

void Application::endFrame()
{
}

void Application::mainLoop()
{
    uint32_t now = getTicks();
    m_timeSinceLastFrame = now - m_currentFrameTime;
    m_currentFrameTime = now;

    beginFrame();
    handleInput();
    update();
    render();
    endFrame();

    ++m_totalFrames;
    updateFPSData();

#if !defined (__EMSCRIPTEN__)
    uint32_t frameLength = getTicks() - m_currentFrameTime;
    if (frameLength < m_desiredFrameTime)
    {
        this_thread::sleep_for(chrono::milliseconds(m_desiredFrameTime - frameLength));
    }
#endif
}

void Application::updateFPSData()
{
    const uint32_t now = getTicks();
    const uint32_t fpsStatusUpdateTimeDelta = now - m_lastFPSStatusUpdateTime;

    if (fpsStatusUpdateTimeDelta > 1000) // update once per second
    {
        const uint32_t framesDelta = m_totalFrames - m_lastFPSStatusUpdateFrame;
        m_meanFrameTime = fpsStatusUpdateTimeDelta / framesDelta;
        m_fps = (framesDelta * 1000) / fpsStatusUpdateTimeDelta;

        m_lastFPSStatusUpdateTime = now;
        m_lastFPSStatusUpdateFrame = m_totalFrames;

        //ostringstream sout;
        //sout << "mean frame time: " << m_meanFrameTime << " ms | fps: " << m_fps;
        //m_mainWindow->setTitle(sout.str().c_str());
    }
}

uint32_t Application::getTicks() const
{
    // actually the time returned is since the first time this function is called 
    // but this is very early in the execution time, so it's fine

    static auto start = high_res_clock::now();
    auto time = high_res_clock::now() - start;
    return uint32_t(chrono::duration_cast<chrono::milliseconds>(time).count()) + 1;
}
