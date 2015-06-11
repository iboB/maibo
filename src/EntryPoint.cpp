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

int main(int argc, char* argv[])
{
    Application_CreateInstance(argc, argv);
    
    Application& theApp = Application_Instance();

    theApp.initialize();

    Application_DestroyInstance(0);

    return 0;
}