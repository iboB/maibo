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
#include <maibo/AppState.h>

#include <sstream>

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
        return false;
    }
#endif

    TaskManager::instance().setNumTasksPerUpdate(cp.numTasksPerUpdate);

    return true;
}

void Application::deinitialize()
{
    m_currentState->deinitialize();
    safe_delete(m_currentState);
    safe_delete(m_nextState);

    safe_delete(m_mainWindow);

    SDL_Quit();
}

void Application::beginFrame()
{
    m_currentState->beginFrame();
}

void Application::handleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // let the state handle events with top priority
        if (m_currentState->handleEvent(event))
        {
            continue;
        }

        if (event.type == SDL_QUIT)
        {
            m_isRunning = false;
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_isRunning = false;
                break;
#if defined(MAIBO_PLATFORM_DESKTOP)
            case SDLK_p:
                m_isWireframe = !m_isWireframe;
                glPolygonMode(GL_FRONT_AND_BACK, m_isWireframe ? GL_LINE : GL_FILL);
                break;
#endif
            }
        }


    }
}

void Application::update()
{
    TaskManager::instance().update();
    m_currentState->update(m_timeSinceLastFrame);
}

void Application::render()
{
    m_currentState->render();
}

void Application::endFrame()
{
    m_currentState->render();
    m_mainWindow->swapBuffers();
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

    checkForStateChange();

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

        ostringstream sout;
        sout << "ft: " << m_meanFrameTime << " ms | fps: " << m_fps;
        m_mainWindow->setTitle(sout.str().c_str());
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

void Application::setState(AppState* newState)
{
    if (m_currentState)
    {
        // if there is an existing state finish its cycle, and then 
        // set the new state on the next update
        if (m_nextState)
        {
            m_nextState->deinitialize();
            delete m_nextState;
        }

        m_nextState = newState;
    }
    else
    {
        assert(!m_nextState);
        
        if (!newState->initialize())
        {
            onSetStateError(newState);
        }
        else
        {
            m_currentState = newState;
        }
    }
}

void Application::checkForStateChange()
{
    if (m_nextState)
    {
        // there is a next state to be set
        // finish with the current one and set the next

        assert(m_currentState);
        m_currentState->deinitialize();
        
        safe_delete(m_currentState);

        if (!m_nextState->initialize())
        {
            onSetStateError(m_nextState);
        }
        else
        {
            m_currentState = m_nextState;
        }
        
        m_nextState = nullptr;
    }
}
