// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Main application class
// Users must inherit from it and override its virtual methods

namespace maibo
{
    class Application
    {
    protected:
        Application();
        virtual ~Application();
    
    public:
        virtual bool initialize() = 0;
    };

    // Users must give body to these functions, creating, returning, and destroying
    // an instance of their own Application class
    extern void Application_CreateInstance(int argc, char* argv[]);
    extern Application& Application_Instance();
    extern void Application_DestroyInstance(int retCode);
}