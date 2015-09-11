// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/InputEventHandler.h>
#include <maibo/lib/Signals/AutoSlot.h>

namespace Rocket
{
    namespace Core
    {
        class Context;
        class ElementDocument;
    }
}

namespace maibo
{
    class LibRocketLayer : public AutoSlot, public InputEventHandler
    {
    public:
        LibRocketLayer(const std::string& name);
        ~LibRocketLayer();

        slot update(uint32_t dt);
        slot render();

        // from imput event handler
        virtual bool handleEvent(const SDL_Event& event) override;

        void loadRootRml(const std::string& rmlPath);

    private:
        Rocket::Core::Context* m_rocketContext = nullptr;
        Rocket::Core::ElementDocument* m_rootDocument = nullptr;

        const std::string m_name;
    };
}
