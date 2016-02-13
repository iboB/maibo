// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/GUI/LibRocket/LibRocketLayer.h>
#include <maibo/GUI/LibRocket/LibRocketManager.h>

#include <maibo/RenderTargetManager.h>
#include <maibo/Application.h>
#include <maibo/lib/GLSentries.h>

#include <Rocket/Core.h>

using namespace mathgp;
using namespace std;
using namespace maibo;

LibRocketLayer::LibRocketLayer(const std::string& name)
    : m_name(name)
{
    const auto& manager = LibRocketManager::instance();

    const auto& screenSize = RenderTargetManager::instance().currentRenderTargetSize();

    m_rocketContext = Rocket::Core::CreateContext(m_name.c_str(),
        Rocket::Core::Vector2i(int(screenSize.x()), int(screenSize.y()))
        );

    auto& app = Application_Instance();

    app.OnPreUpdate.connect<LibRocketLayer, &LibRocketLayer::update>(this);
    app.OnPostRender.connect<LibRocketLayer, &LibRocketLayer::render>(this);
    app.addGlobalInputEventHandler(this);
}

LibRocketLayer::~LibRocketLayer()
{
    auto& app = Application_Instance();
    app.removeGlobalInputEventHandler(this);

    if (m_rootDocument)
    {
        m_rootDocument->RemoveReference();
        m_rootDocument = nullptr;
    }

    m_rocketContext->RemoveReference();
    m_rocketContext = nullptr;
}

slot LibRocketLayer::update(uint32_t)
{
    m_rocketContext->Update();
}

slot LibRocketLayer::render()
{
    MAIBO_GL_SENTRY(GLEnable, GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    MAIBO_GL_SENTRY(GLDisable, GL_DEPTH_TEST);

    m_rocketContext->Render();
}

void LibRocketLayer::loadRootRml(const std::string& rmlPath)
{
    assert(m_rocketContext);

    m_rootDocument = m_rocketContext->LoadDocument(rmlPath.c_str());

    if (!m_rootDocument)
    {
        cout << "couldn't load rml: " << rmlPath << endl;
        assert(false);
        return;
    }

    m_rootDocument->Show();
}

bool LibRocketLayer::handleEvent(const SDL_Event& event)
{
    // process all mouse events (for now)

    if (event.type == SDL_MOUSEMOTION)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        m_rocketContext->ProcessMouseMove(x, y, 0);

        return true;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_rocketContext->ProcessMouseButtonDown(event.button.button - 1, 0);
        return true;
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        m_rocketContext->ProcessMouseButtonUp(event.button.button - 1, 0);
        return true;
    }

    return false;
}

