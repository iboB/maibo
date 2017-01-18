// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Common/Signals/Signal.h"

// Main application class
// Users must inherit from it and override its virtual methods

namespace maibo
{
    class MainWindow;
    class AppState;
    class InputEventHandler;

    class Application
    {
    protected:
        Application();
        virtual ~Application();

    public:
        bool isRunning() const { return m_isRunning; }

        // Returns false on error
        virtual bool initialize() = 0;
        virtual void deinitialize();

        // Function to be called for each appliacation frame
        void mainLoop();

        const MainWindow& mainWindow() const { return *m_mainWindow; }

    public:

        Signal<> OnBeginFrame;
        Signal<uint32_t> OnPreUpdate;
        Signal<uint32_t> OnPostUpdate;
        Signal<> OnPreRender;
        Signal<> OnPostRender;
        Signal<> OnEndFrame;

    protected:

        void startRunning() { m_isRunning = true; }
        void stopRunning() { m_isRunning = false; }

        virtual void beginFrame();
        virtual void handleInput();
        virtual void update();
        virtual void render();
        virtual void endFrame();

        struct CreationParameters
        {
            // number of "async" tasks to execute per Application::update
            uint32_t numTasksPerUpdate = 1;

            // desired frame time in milliseconds
            // ignored in emscripten (frame-rate is fixed by the browser)
            // 0 means the fastest possible framerate
            uint32_t desiredFrameTimeMs = 0;

            ///////////////////////////////
            // window creation
            bool createMainWindow = true; // will there be a main window?
            const char* mainWindowTitle = "MaiBo App";
            yama::uvector2 mainWindowClientAreaSize = yama::vt(800u, 600u);
            bool isFullScreen = false;

            bool activateMainWindowAsRenderTarget = true;
        };

        // actual initialization function
        // you must call it from you override of initialize()
        // return false on error
        bool initialize(const CreationParameters& creationParameters);

        MainWindow* m_mainWindow = nullptr;

    private:
        bool m_isRunning = false;

        // wireframe rendering
        // not supported on WebGL or GL ES platforms
        bool m_isWireframe = false;

        //////////////////////////////////////
        // states
    public:
        void setState(AppState* state);

        // called when a state, set to the application failed to initialize
        // it's parameter is the failed state
        // WARNING: you must set a valid state if the state change fails
        // the current application state will be set to nullptr
        virtual void onSetStateError(AppState* state) = 0;

        // forcefully destroys current and pending states
        // called by MaiBo before deinitializing the application
        // if you call this yourself, be careful to set a valid state afterwards
        void destroyStates();

    private:
        // check if there is a state pending to be set
        void checkForStateChange();

        AppState* m_currentState = nullptr;
        AppState* m_nextState = nullptr;

        //////////////////////////////////////
        // events
    public:
        // Adds/Remove global event handlers
        // Will sort them by priority
        void addGlobalInputEventHandler(InputEventHandler* handler);
        void removeGlobalInputEventHandler(InputEventHandler* handler);

    private:
        std::vector<InputEventHandler*> m_globalInputEventHandlers;

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