#pragma once

#include <maibo/Application.h>

// main application class

class MyApplication : public maibo::Application
{
public:
    virtual bool initialize() override;
    virtual void deinitialize() override;
    virtual void render() override;

    virtual void update() override;

    virtual void onSetStateError(maibo::AppState* state) override;
};
