// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "MainMenuState.h"

#include <imgui.h>

using namespace maibo;

bool MainMenuState::initialize()
{
    glClearColor(0.1f, 0.3f, 0.1f, 1);

    return true;
}

void MainMenuState::deinitialize()
{
}

void MainMenuState::beginFrame()
{

}

void MainMenuState::update(uint32_t dt)
{
    static bool show = true;
    ImGui::ShowTestWindow(&show);
}

void MainMenuState::render()
{

}

void MainMenuState::endFrame()
{

}

