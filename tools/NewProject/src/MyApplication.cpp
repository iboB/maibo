#include "MyApplication.h"

#include <maibo/ResourceManager.h>

#include "MyState.h"

using namespace std;
using namespace maibo;

bool MyApplication::initialize()
{
    Application::CreationParameters params;

    params.desiredFrameTimeMs = 10;

    if (!Application::initialize(params))
    {
        return false;
    }

    // You can configure opengl here
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.1f, 0.4f, 1);

    setState(new MyState);

    startRunning();

    return true;
}

void MyApplication::deinitialize()
{
    Application::deinitialize();

    cout << "Total frames: " << totalFrames() << endl;
}

void MyApplication::update()
{
    Application::update();
}

void MyApplication::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Application::render();
}

void MyApplication::onSetStateError(AppState* state)
{
    // nothing to do yet
}

// maibo integration
namespace maibo
{
    namespace
    {
        MyApplication* app = nullptr;
    }

    void Application_CreateInstance(int argc, char* argv[])
    {
        app = new MyApplication();
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
