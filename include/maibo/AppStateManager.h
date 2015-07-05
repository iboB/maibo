// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// App states manager

#include "lib/ExplicitSingleton.h"

namespace maibo
{
    class AppState;

    class AppStateManager : public ExplicitSingleton < AppStateManager >
    {
        MAIBO_DECLARE_EXPLICIT_SINGLETON(AppStateManager);
    public:

        void setState(AppState* newState);

        bool processEvent(const SDL_Event& event); // returns true if any of the states returned true in handleEvent
        void updateStates(int dt);
        void drawFrame();

    protected:
        AppState* m_currentState;
        AppState* m_nextState;
    };
}
