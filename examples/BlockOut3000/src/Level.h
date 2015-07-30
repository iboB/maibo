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
    ~Level();

    void createBuffers();

    void render();

    const mathgp::matrix& viewTransform() const { return m_viewTransfrosm; }

    char& at(unsigned layer, unsigned row, unsigned col) { return m_levelData[indexInData(layer, row, col)]; }
    char at(unsigned layer, unsigned row, unsigned col) const { return m_levelData[indexInData(layer, row, col)]; }

private:
    // logical data
    const mathgp::uvector3 m_size;

    unsigned indexInData(unsigned layer, unsigned row, unsigned col) const { return layer * m_size.x() * m_size.y() + row * m_size.x() + col; }
    char* m_levelData;

    // physical data
    GLuint m_wireBuffer = 0;
    unsigned m_numWireVertices = 0;
    GLuint m_solidBuffer = 0;
    unsigned m_numSolidVertices = 0;

    // we wat the camera to look at 000
    // since the level takes dimensions from 000 to whd 
    // we need this matrix to transform it to the appropriate place
    mathgp::matrix m_viewTransfrosm;
};
