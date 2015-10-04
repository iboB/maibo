// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/GUI/LibRocket/LibRocketSystemInterface.h>

#include <maibo/Application.h>

using namespace maibo;
using namespace std;

float LibRocketSystemInterface::GetElapsedTime()
{
    return float(Application_Instance().getTicks()) / 1000;
}

bool LibRocketSystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
    {
        cout << "libRocket log: " << message.CString() << endl;
    }

    return type <= Rocket::Core::Log::LT_ASSERT;
}