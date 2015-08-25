// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// base class for render targets

namespace maibo
{
    class RenderTarget
    {
    public:
        // you need to override this method to get the surface size
        virtual const mathgp::uvector2& surfaseSize() const = 0;

        // you need to call this implementation in your overrides
        // it sets itself as the current render target in the render target manager
        virtual void activate();

    protected:
        // shouldn't be used in arrays, hence the protected destructor
        virtual ~RenderTarget();
    };
}
