// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "BlockOut.h"

using namespace std;

bool BlockOut::initialize()
{
    cout << "Blook!!!" << endl;

    return true;
}

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
