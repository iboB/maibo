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
    if (!Application::initialize())
    {
        return false;
    }

    //startRunning();

    cout << sizeof(high_res_clock::rep) << endl;

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
