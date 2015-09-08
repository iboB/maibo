// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <Rocket/Core/SystemInterface.h>

namespace maibo
{
    class LibRocketSystemInterface : public Rocket::Core::SystemInterface
    {
    public:
        virtual float GetElapsedTime() override;
        virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message) override;
    };
}
