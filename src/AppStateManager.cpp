// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

// Game states manager
#include <maibo/AppStateManager.h>
#include <maibo/AppState.h>

using namespace maibo;

AppStateManager::AppStateManager()
    : m_currentState(nullptr)
    , m_nextState(nullptr)
{

}

AppStateManager::~AppStateManager()
{
    m_currentState->deinitialize();
    delete m_currentState;

    delete m_nextState;
}

void AppStateManager::setState(AppState* newState)
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
        m_currentState = newState;
        m_currentState->initialize();
    }    
}

bool AppStateManager::processEvent(const SDL_Event& event)
{
    return m_currentState->handleEvent(event);
}

void AppStateManager::updateStates(int dt)
{
    if (m_nextState)
    {
        // there is a next state to be set
        // finish with the current one and set the next

        assert(m_currentState);
        m_currentState->deinitialize();
        delete m_currentState;

        m_currentState = m_nextState;
        m_currentState->initialize();

        m_nextState = nullptr;
    }

    m_currentState->update(dt);
}

void AppStateManager::drawFrame()
{
    m_currentState->draw();
}
