// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/GUI/LibRocket/LibRocketManager.h>
#include <maibo/GUI/LibRocket/LibRocketSystemInterface.h>
#include <maibo/GUI/LibRocket/LibRocketRenderInterface.h>

#include <Rocket/Core/Core.h>

using namespace maibo;
using namespace std;

LibRocketManager::LibRocketManager()
{
    m_systemInterface = new LibRocketSystemInterface;
    Rocket::Core::SetSystemInterface(m_systemInterface);

    m_renderInterface = new LibRocketRenderInterface;
    Rocket::Core::SetRenderInterface(m_renderInterface);

    if (!Rocket::Core::Initialise())
    {
        assert(false);
        cout << "couldn't initialize libRocket" << endl;
    }
}

LibRocketManager::~LibRocketManager()
{
    Rocket::Core::Shutdown();
}

bool LibRocketManager::loadFont(const char* fontFilename)
{
    return Rocket::Core::FontDatabase::LoadFontFace(fontFilename);
}
