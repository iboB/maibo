// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Base class for input event handlers

namespace maibo
{
    class InputEventHandler
    {
    public:
        virtual ~InputEventHandler() {}

        // return value shows whether the event has been processed by this handler
        virtual bool handleEvent(const SDL_Event&) = 0;

        int priority() const { return m_priority; }

    protected:
        // priority of the handler, to be compared by siblings in the handling loop
        int m_priority;
    };
}
