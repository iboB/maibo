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
#include "Resources.h"

bool LoadAllState::initialize()
{
    auto& rm = maibo::ResourceManager::instance();
    m_uniformColorFuture = rm.loadGPUProgramAsync("resources/pos.vert", "resources/u_color.vert", true);
    addResourceFuture(m_uniformColorFuture);

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

    r.uniformColorProgram = m_uniformColorFuture->resource();

    maibo::Application_Instance().setState(new PlayingState);
}
