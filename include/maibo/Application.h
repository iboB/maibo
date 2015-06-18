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
        bool isRunning() const { return m_isRunning; }

        // Returns false on error
        virtual bool initialize();
        virtual void deinitialize();

        // Function to be called for each appliacation frame
        void mainLoop();

        // total number of frames the aplication has run
        size_t totalFrames() const { return m_totalFrames; }

    protected:
        void startRunning() { m_isRunning = true; }
        void stopRunning() { m_isRunning = false; }

        virtual void beginFrame();
        virtual void handleInput();
        virtual void update();
        virtual void render();
        virtual void endFrame();

    private:
        bool m_isRunning = false;

        size_t m_totalFrames = 0;
    };

    // Users must give body to these functions, creating, returning, and destroying
    // an instance of their own Application class
    extern void Application_CreateInstance(int argc, char* argv[]);
    extern Application& Application_Instance();
    extern void Application_DestroyInstance(int retCode);
}