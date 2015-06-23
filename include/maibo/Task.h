// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Base interface for asynchronous tasks

namespace maibo
{
    class Task
    {
    public:
        virtual ~Task() {}

        // Return true if the task has been complete on this call of execute
        // Return false if execute needs to be called again on the next available opportunity
        virtual bool execute() = 0;
    };
}
