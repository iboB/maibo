// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PlayingState.h"

// main gameplay state

bool PlayingState::initialize()
{
    glClearColor(0.0f, 0.1f, 0.4f, 1);
    return true;
}

void PlayingState::deinitialize()
{

}

bool PlayingState::handleEvent(const SDL_Event&)
{
    return false;
}

void PlayingState::beginFrame()
{

}

void PlayingState::update(uint32_t dt)
{

}

void PlayingState::render()
{

}

void PlayingState::endFrame()
{

}
