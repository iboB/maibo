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

        //////////////////////////////////////
        // time and fps stuff
    public:

        // time in ms since the application has started
        uint32_t getTicks() const;

        // total number of frames the aplication has run
        uint32_t totalFrames() const { return m_totalFrames; }
    private:
        uint32_t m_currentFrameTime = 0; // start of current frame (ms)
        uint32_t m_timeSinceLastFrame = 0; // used as delta for updates

        uint32_t m_desiredFrameTime = 0; // the application will try to work at this time per frame

        void updateFPSData();

        uint32_t m_totalFrames = 0; // number of frames since the app has started
        uint32_t m_lastFrameEnd = 0; // end time of the last frame
        uint32_t m_lastFPSStatusUpdateTime = 0; // last time the fps status was updated
        uint32_t m_lastFPSStatusUpdateFrame = 0; // frame number of the last time the status was updated

        // results
        uint32_t m_meanFrameTime;
        uint32_t m_fps;
    };

    // Users must give body to these functions, creating, returning, and destroying
    // an instance of their own Application class
    extern void Application_CreateInstance(int argc, char* argv[]);
    extern Application& Application_Instance();
    extern void Application_DestroyInstance(int retCode);
}