// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Application.h>

// Game level class

class Level
{
public:
    Level(const mathgp::uvector3& size);

    void createBuffers();

    void render();

    const mathgp::matrix& viewTransform() const { return m_viewTransfrosm; }

private:
    GLuint m_wireBuffer;
    unsigned m_numWireVertices;
    GLuint m_solidBuffer;
    unsigned m_numSolidVertices;

    const mathgp::uvector3 m_size;

    // we wat the camera to look at 000
    // since the level takes dimensions from 000 to whd 
    // we need this matrix to transform it to the appropriate place
    mathgp::matrix m_viewTransfrosm;
};
