// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// base class for application states

namespace maibo
{
    class AppState
    {
    public:
        virtual ~AppState() {}

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;

        // not pure virtual since you may want to create a state with no input
        // return value is whether the event should be processed by others
        virtual bool handleEvent(const SDL_Event&) { return false; };

        // dt is time delta since last frame    
        virtual void update(uint32_t dt) = 0;

        virtual void draw() = 0;
    };
}
