// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/Application.h>

using namespace maibo;

Application::Application()
{
}

Application::~Application()
{
}

bool Application::initialize()
{
    return true;
}

void Application::deinitialize()
{

}

void Application::beginFrame()
{

}

void Application::handleInput()
{

}

void Application::update()
{

}

void Application::render()
{

}

void Application::endFrame()
{

}

void Application::mainLoop()
{
    beginFrame();
    handleInput();
    update();
    render();
    endFrame();
}