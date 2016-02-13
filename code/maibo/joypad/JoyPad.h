// MaiBo - JoyPad
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

namespace maibo
{
namespace joypad
{

class JoyPad
{
public:
    JoyPad();
    ~JoyPad();

    // return true if the event has been processed by the joypad
    bool processExternalEvent(SDL_Event& event);
};

}
}
