// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Application.h"

#if defined(_WIN32)
#   include <Windows.h>
#   include <stdio.h>
#   include <io.h>
#   include <fcntl.h>
#endif

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
    theApp.destroyStates();
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
#if defined(_WIN32)
    AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((intptr_t)handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
#endif

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