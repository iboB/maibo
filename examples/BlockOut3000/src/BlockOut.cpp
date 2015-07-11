// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "BlockOut.h"

#include <maibo/ResourceManager.h>
#include <maibo/lib/high_res_clock.h>

using namespace std;
using namespace maibo;


bool BlockOut::initialize()
{
    Application::CreationParameters params;

    if (!Application::initialize(params))
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST); // z buffer
    glEnable(GL_CULL_FACE); // cull back (CW) faces

    glClearColor(0.0f, 0.1f, 0.4f, 1); // backbuffer clear color
    glClearDepth(1); // z buffer clear value

    startRunning();

    return true;
}

void BlockOut::deinitialize()
{
    Application::deinitialize();

    cout << "Total frames: " << totalFrames() << endl;
}

void BlockOut::update()
{
    Application::update();
}

void BlockOut::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Application::render();
}

void BlockOut::onSetStateError(AppState* state)
{
    // nothing to do yet
}

// maibo integration
namespace maibo
{
    namespace
    {
        BlockOut* app = nullptr;
    }

    void Application_CreateInstance(int argc, char* argv[])
    {
        app = new BlockOut();
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
