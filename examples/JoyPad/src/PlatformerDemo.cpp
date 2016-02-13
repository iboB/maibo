// MaiBo - JoyPad
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PlatformerDemo.h"

#include <maibo/Application.h>

using namespace maibo;

bool PlatformerDemo::initialize()
{
    glClearColor(0.1f, 0.3f, 0.6f, 1);

    return true;
}

void PlatformerDemo::deinitialize()
{
}

bool PlatformerDemo::handleEvent(const SDL_Event& event)
{
    return false;
}

void PlatformerDemo::beginFrame()
{
}

void PlatformerDemo::update(uint32_t dt)
{
}

void PlatformerDemo::render()
{
}

void PlatformerDemo::endFrame()
{
}

