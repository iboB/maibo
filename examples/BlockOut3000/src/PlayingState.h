// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/AppState.h>

// main gameplay state

class Level;
class Figure;

class PlayingState : public maibo::AppState
{
public:
    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual bool handleEvent(const SDL_Event&) override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;

private:
    Level* m_level = nullptr;
    Figure* m_currentFigure = nullptr; // figure that is currently falling in the level
};
