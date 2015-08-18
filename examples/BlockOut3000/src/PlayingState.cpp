// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PlayingState.h"

#include "Resources.h"
#include "Level.h"
#include "FigureManager.h"
#include "FigureSet.h"
#include "FigureTemplate.h"
#include "Figure.h"

#include <maibo/GPUProgram.h>
#include <maibo/lib/GLSentries.h>

#include <imgui.h>

// main gameplay state

using namespace mathgp;
using namespace maibo;

GLuint buffer;

bool PlayingState::initialize()
{
    glClearColor(0.0f, 0.1f, 0.4f, 1);

    m_level = new Level(v(5u, 5u, 10u));
    m_level->createBuffers();

    m_figureSet = FigureManager::instance().getFigureSet("Flat");

    m_nextFigure = new Figure(*m_figureSet->getRandomFigureTemplate(), *m_level);
    spawnNextFigure();

    return true;
}

void PlayingState::deinitialize()
{
    safe_delete(m_currentFigure);
    safe_delete(m_nextFigure);
    safe_delete(m_level);
}

bool PlayingState::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_q:
            m_currentFigure->tryRotateX(1);
            return true;
        case SDLK_a:
            m_currentFigure->tryRotateX(-1);
            return true;
        case SDLK_w:
            m_currentFigure->tryRotateY(1);
            return true;
        case SDLK_s:
            m_currentFigure->tryRotateY(-1);
            return true;
        case SDLK_e:
            m_currentFigure->tryRotateZ(1);
            return true;
        case SDLK_d:
            m_currentFigure->tryRotateZ(-1);
            return true;
        case SDLK_LEFT:
            m_currentFigure->tryMove(vc(-1, 0, 0));
            return true;
        case SDLK_RIGHT:
            m_currentFigure->tryMove(vc(1, 0, 0));
            return true;
        case SDLK_UP:
            m_currentFigure->tryMove(vc(0, 1, 0));
            return true;
        case SDLK_DOWN:
            m_currentFigure->tryMove(vc(0, -1, 0));
            return true;
        case SDLK_SPACE:
            m_currentFigure->tryMove(vc(0, 0, -1));
            return true;
        }
    }

    return false;
}

void PlayingState::beginFrame()
{

}

void PlayingState::update(uint32_t dt)
{
    m_currentFigure->update(dt);
    m_level->update(dt);

    if (m_currentFigure->isFallen())
        spawnNextFigure();

    static bool show = true;
    ImGui::ShowTestWindow(&show);
}

void PlayingState::render()
{
    auto& mat = Resources::instance().uniformColorMaterial;
    mat.begin();

    matrix proj = matrix::perspective_fov_rh(mathgp::constants<float>::PI() / 2.3f, 800.f/600.f, 1, 100);
    matrix view = matrix::look_towards_rh(vc(0, 0, 1.3f), vc(0, 0, -1), vc(0, 1, 0));
    matrix projView = proj * view * m_level->viewTransform();

    mat.setProjView(projView);

    m_level->draw();

    m_currentFigure->draw();

    mat.end();
}

void PlayingState::endFrame()
{
}

void PlayingState::spawnNextFigure()
{
    delete m_currentFigure;
    m_currentFigure = m_nextFigure;
    m_nextFigure = new Figure(*m_figureSet->getRandomFigureTemplate(), *m_level);
    m_currentFigure->spawn();
}
