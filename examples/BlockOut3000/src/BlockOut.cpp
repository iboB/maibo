// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "BlockOut.h"

#include <maibo/ResourceManager.h>

using namespace std;
using namespace maibo;

ResourceFuturePtr<int> future;

bool BlockOut::initialize()
{
    if (!Application::initialize())
    {
        return false;
    }

    startRunning();

    future = ResourceManager::instance().GetFileAsync("test.txt");

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

    if (future->isDone())
    {
        cout << "Task done with result " << future->resource() << endl;
        stopRunning();
    }
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
