// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/ImplicitSingleton.h"

// render targer manager
// provides basic info about the current render target

namespace maibo
{
    class RenderTarget;

    class RenderTargetManager : public ImplicitSingleton<RenderTargetManager>
    {
    public:
        void setScreenRenderTarget(RenderTarget* rt);
        const RenderTarget* screenRenderTarget() const { return m_screenRenderTarget; }
        void activateScreenRenderTarget();

        RenderTarget* getCurrentRenderTarget() { return m_currentRenderTarget; }

        // shorthand to get the size without having to include RenderTarget.h
        const mathgp::uvector2& currentRenderTargetSize() const;

    private:
        friend class RenderTarget;

        // "main" render target (currently this can only be the MainWindow)
        RenderTarget* m_screenRenderTarget = nullptr;

        // render target that has been activated last
        RenderTarget* m_currentRenderTarget = nullptr;
    };
}
