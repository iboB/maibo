// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PlayingState.h"

#include "Resources.h"

#include <maibo/GPUProgram.h>
#include <maibo/lib/GLSentries.h>

// main gameplay state

using namespace mathgp;
using namespace maibo;

GLuint buffer;
GLint pvmLocation;
GLint colorLocation;
uint32_t vpos;

bool PlayingState::initialize()
{
    glClearColor(0.0f, 0.1f, 0.4f, 1);

    auto prog = Resources::instance().uniformColorProgram.get();
    vpos = prog->bindCustomAttribute("v_pos");
    
    pvmLocation = prog->getParameterByName("pvm");
    colorLocation = prog->getParameterByName("color");

    vector3 vertices[] = {
        vc(0, 0, 0.1f),
        vc(1, 0, 0.1f),
        vc(0, 1, 0.1f),
    };

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return true;
}

void PlayingState::deinitialize()
{

}

bool PlayingState::handleEvent(const SDL_Event&)
{
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
    auto prog = Resources::instance().uniformColorProgram.get();
    prog->use();

    prog->setParameter(colorLocation, vc(0.4f, 1, 0.4f, 1));
    prog->setParameter(pvmLocation, matrix::identity());

    MAIBO_GL_SENTRY(GLEnableAttrib, vpos);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vector3), OFFSET(0));

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void PlayingState::endFrame()
{
}
