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
#include "FigureTemplate.h"

#include <maibo/GPUProgram.h>
#include <maibo/lib/GLSentries.h>

// main gameplay state

using namespace mathgp;
using namespace maibo;

GLuint buffer;

bool PlayingState::initialize()
{
    glClearColor(0.0f, 0.1f, 0.4f, 1);

    m_level = new Level(v(5u, 5u, 10u));
    m_level->createBuffers();

    return true;
}

void PlayingState::deinitialize()
{
    safe_delete(m_level);
}

bool PlayingState::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
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

}

void PlayingState::render()
{
    auto& mat = Resources::instance().uniformColorMaterial;
    mat.begin();
    // mat.setColor(vc(0.4f, 1, 0.4f, 1));

    matrix proj = matrix::perspective_fov_rh(mathgp::constants<float>::PI() / 2.3f, 800.f/600.f, 1, 100);
    matrix view = matrix::look_towards_rh(vc(0, 0, 1.3f), vc(0, 0, -1), vc(0, 1, 0));
    matrix projView = proj * view * m_level->viewTransform();

    mat.setProjView(projView);

    m_level->render();

    matrix figureTransform = matrix::translation(0, 0, 8);
    mat.setModel(figureTransform);

    FigureManager::instance().m_allFigureTemplates[1]->draw(vc(0.9f, 0.5f, 0.5f, 0.1f), vc(1, 1, 1, 1));

    mat.end();
}

void PlayingState::endFrame()
{
}
