// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/Application.h>

// Entry point for MaiBo applications

using namespace maibo;

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>

void BreakMainLoop()
{
    emscripten_cancel_main_loop();
}

#else

bool RunMainLoop = false;
void BreakMainLoop()
{
    RunMainLoop = false;
}

#endif

void Exit(int exitCode)
{
    BreakMainLoop();
    Application& theApp = Application_Instance();
    theApp.deinitialize();
    Application_DestroyInstance(exitCode);
}

void MainLoop()
{
    Application& theApp = Application_Instance();

    if (theApp.isRunning())
    {
        theApp.mainLoop();
    }
    else
    {
        Exit(0);
    }
}

int main(int argc, char* argv[])
{
    Application_CreateInstance(argc, argv);
    
    Application& theApp = Application_Instance();

    theApp.initialize();

    // main loop
#if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(MainLoop, 0, 1);
#else
    RunMainLoop = theApp.isRunning();
    while (RunMainLoop)
    {
        MainLoop();
    }
#endif

    // WARNING:
    // Do not be tempted to add code here
    // In Emscripten it will be executed before even starting the main loop

    // For finalization code use the function Exit in this file

    return 0;
}