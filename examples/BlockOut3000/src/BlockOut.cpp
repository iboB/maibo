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

ResourceFuturePtr<vector<char>> future;

bool BlockOut::initialize()
{
    if (!Application::initialize())
    {
        return false;
    }

    startRunning();

    future = ResourceManager::instance().ReadFileAsync("test.txt", true);

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
        cout << "Task done with result: " << endl;
        for (auto c : future->resource())
        {
            cout << c;
        }
        cout << endl;

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
