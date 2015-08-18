// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// base class and interface for managers
// a manager is basically an independent object that can be added to the application
// and its methods will be called at appropriate times

namespace maibo
{
    class Manager
    {
    public:
        virtual ~Manager() {}

        virtual const char* name() const = 0;

        virtual bool initialize() { return true; }
        virtual void deinitialize() {}

        virtual void beginFrame() {}
        virtual bool handleEvent(const SDL_Event& event) { return false; }
        virtual void update(uint32_t dt) {}
        virtual void endFrame() {}
    };
}
