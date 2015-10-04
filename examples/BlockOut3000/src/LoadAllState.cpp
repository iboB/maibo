// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadAllState.h"

#include <maibo/Application.h>
#include <maibo/ResourceManager.h>

#include "PlayingState.h"
#include "MainMenuState.h"
#include "Resources.h"
#include "FigureManager.h"
#include "LoadFigureSetsTask.h"
#include <maibo/TaskManager.h>
#include <maibo/GUI/LibRocket/LibRocketManager.h>

namespace
{
    const char* const MainFont_Filename = "resources/gui/fonts/atari.ttf";
}

bool LoadAllState::initialize()
{
    auto& rm = maibo::ResourceManager::instance();

    m_uniformColorFuture = rm.loadGPUProgramAsync("resources/pos.vert", "resources/u_color.frag", true);
    addFuture(m_uniformColorFuture);

    m_figureDataFuture = rm.readFileAsync("resources/figures.dat", true);
    addFuture(m_figureDataFuture);

    auto task = new LoadFigureSetsTask(m_figureDataFuture);
    maibo::TaskManager::instance().pushTask(task);
    addFuture(task->future);

    rm.getFileAsync(MainFont_Filename);
    rm.getFileAsync("resources/gui/main.css");
    rm.getFileAsync("resources/gui/main.xml");

    glClearColor(1, 0.1f, 0.4f, 1);

    return true;
}

void LoadAllState::deinitialize()
{

}

void LoadAllState::beginFrame()
{

}

void LoadAllState::update(uint32_t dt)
{
    maibo::ResourceFutureState::update(dt);
}

void LoadAllState::render()
{

}

void LoadAllState::endFrame()
{

}

void LoadAllState::onDone()
{
    auto& r = Resources::instance();

    r.uniformColorMaterial.m_program = m_uniformColorFuture->resource();
    r.uniformColorMaterial.initialize();

    FigureManager::instance().prepareFigureTemplatesPhysicalData();

    maibo::LibRocketManager::instance().loadFont(MainFont_Filename);

    maibo::Application_Instance().setState(new PlayingState);
    //maibo::Application_Instance().setState(new MainMenuState);
}
