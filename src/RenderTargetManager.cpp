// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/RenderTargetManager.h>
#include <maibo/RenderTarget.h>

using namespace maibo;

void RenderTargetManager::activateScreenRenderTarget()
{
    assert(m_screenRenderTarget);
    m_screenRenderTarget->activate();
}

void RenderTargetManager::setScreenRenderTarget(RenderTarget* rt)
{
    // we don't support changing of main render target (yet)
    assert(!m_screenRenderTarget);
    m_screenRenderTarget = rt;
}

const mathgp::uvector2& RenderTargetManager::currentRenderTargetSize() const
{
    assert(m_currentRenderTarget);
    return m_currentRenderTarget->surfaseSize();
}
