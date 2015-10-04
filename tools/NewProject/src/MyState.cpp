#include "MyState.h"

using namespace maibo;

bool MyState::initialize()
{
    glClearColor(0.1f, 0.3f, 0.1f, 1);

    return true;
}

void MyState::deinitialize()
{
}

void MyState::beginFrame()
{
}

void MyState::update(uint32_t dt)
{
}

void MyState::render()
{
}

void MyState::endFrame()
{
}

