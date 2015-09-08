// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/lib/ExplicitSingleton.h>

// LibRocket manager
// Used for global stuff with the integration of LibRocket (https://github.com/libRocket/libRocket)

namespace maibo
{
    class LibRocketSystemInterface;
    class LibRocketRenderInterface;

    class LibRocketManager : public ExplicitSingleton<LibRocketManager>
    {
        MAIBO_DECLARE_EXPLICIT_SINGLETON(LibRocketManager);
    public:

        bool loadFont(const char* fontFilename);

    private:
        LibRocketSystemInterface* m_systemInterface;
        LibRocketRenderInterface* m_renderInterface;
    };
}
