// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadAllState.h"

#include <maibo/Application.h>
#include <maibo/Resources/ResourceManager.h>
#include <maibo/Assets/FileManager.h>

#include "PlayingState.h"
#include "MainMenuState.h"
#include "Resources.h"
#include "FigureManager.h"
#include "LoadFigureSetsTask.h"
#include <maibo/TaskManager.h>

namespace
{
    const char* const MainFont_Filename = "resources/gui/fonts/atari.ttf";
}

bool LoadAllState::initialize()
{
    auto& rm = maibo::ResourceManager::instance();

    m_uniformColorFuture = rm.loadGPUProgramAsync("resources/pos.vert", "resources/u_color.frag", true);
    addFuture(m_uniformColorFuture);

    auto& fm = maibo::FileManager::instance();

    auto figDataFuture = fm.readFileAsync("resources/figures.dat", true);
    addFuture(figDataFuture);

    auto task = new LoadFigureSetsTask(figDataFuture);
    maibo::TaskManager::instance().pushTask(task);
    addFuture(task->future);

    fm.getFileAsync(MainFont_Filename);
    fm.getFileAsync("resources/gui/main.css");
    fm.getFileAsync("resources/gui/main.xml");

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
    maibo::WaitForFuturesState::update(dt);
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

    maibo::Application_Instance().setState(new PlayingState);
    //maibo::Application_Instance().setState(new MainMenuState);
}
