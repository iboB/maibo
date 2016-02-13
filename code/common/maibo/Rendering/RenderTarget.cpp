// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "RenderTarget.h"
#include "RenderTargetManager.h"

using namespace maibo;

RenderTarget::~RenderTarget()
{
    RenderTargetManager::instance().m_currentRenderTarget = nullptr;
}

void RenderTarget::activate()
{
    RenderTargetManager::instance().m_currentRenderTarget = this;
}
