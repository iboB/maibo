// MaiBo - JoyPad
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "JoyPadState.h"

#include "PlatformerDemo.h"

#include <maibo/Application.h>

#include <imgui.h>

using namespace maibo;

bool JoyPadState::initialize()
{
    glClearColor(0.1f, 0.3f, 0.1f, 1);

    return true;
}

void JoyPadState::deinitialize()
{
}

void JoyPadState::beginFrame()
{
}

void JoyPadState::update(uint32_t dt)
{
    ImGui::SetNextWindowPosCenter();
    ImGui::Begin("Choose demo");

    if (ImGui::Button("Platformer demo"))
    {
        Application_Instance().setState(new PlatformerDemo);
    }

    ImGui::End();
}

void JoyPadState::render()
{
}

void JoyPadState::endFrame()
{
}

