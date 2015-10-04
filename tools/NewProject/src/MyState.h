#pragma once

#include <maibo/AppState.h>

class MyState : public maibo::AppState
{
public:
    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;
};
