// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/States/WaitForFuturesState.h>

#include <maibo/Rendering/GPUProgramPtr.h>
#include <maibo/Common/ResourceFuture.h>

// game state that loads all resources for the game

class LoadAllState : public maibo::WaitForFuturesState
{
    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;

    virtual void onDone() override;

    maibo::ResourceFuturePtr<maibo::GPUProgramPtr> m_uniformColorFuture;
};