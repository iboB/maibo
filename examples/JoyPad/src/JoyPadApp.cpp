// MaiBo - JoyPad
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "JoyPadApp.h"

#include <maibo/GUI/ImGui/ImGuiManager.h>

#include "JoyPadState.h"

using namespace std;
using namespace maibo;

bool JoyPadApp::initialize()
{
    Application::CreationParameters params;

    params.desiredFrameTimeMs = 10;
    params.mainWindowClientAreaSize = yama::vt(800u, 480u);

    if (!Application::initialize(params))
    {
        return false;
    }

    // You can configure opengl here
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.1f, 0.4f, 1);

    ImGuiManager::createInstance();

    setState(new JoyPadState);

    startRunning();

    return true;
}

void JoyPadApp::deinitialize()
{
    ImGuiManager::destroyInstance();

    Application::deinitialize();

    cout << "Total frames: " << totalFrames() << endl;
}

void JoyPadApp::update()
{
    Application::update();
}

void JoyPadApp::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Application::render();
}

void JoyPadApp::onSetStateError(AppState* state)
{
    // nothing to do yet
}

// maibo integration
namespace maibo
{
    namespace
    {
        JoyPadApp* app = nullptr;
    }

    void Application_CreateInstance(int argc, char* argv[])
    {
        app = new JoyPadApp();
    }

    Application& Application_Instance()
    {
        return *app;
    }

    void Application_DestroyInstance(int retCode)
    {
        delete app;
    }
}
