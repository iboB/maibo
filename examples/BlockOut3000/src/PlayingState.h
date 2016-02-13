// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/States/AppState.h>

// main gameplay state

class Level;
class LevelLayerPreview;
class Figure;
class FigureSet;

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

    void spawnNextFigure();

    Level* m_level = nullptr;
    LevelLayerPreview* m_layerPreview = nullptr;
    const FigureSet* m_figureSet; // set that is currently being used
    Figure* m_currentFigure = nullptr; // figure that is currently falling in the level
    Figure* m_nextFigure = nullptr; // figure that will be next
};
