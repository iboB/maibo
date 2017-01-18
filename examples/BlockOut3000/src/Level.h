// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Application.h>

class LevelLayer;

// Game level class

class Level
{
public:
    Level(const yama::uvector3& size);
    ~Level();

    void createBuffers();

    void update(uint32_t dt);

    void draw() const;

    const yama::uvector3& size() const { return m_size; }

    const yama::matrix& viewTransform() const { return m_viewTransfrosm; }

    bool canFitFigure(const std::vector<yama::ivector3>& elements) const;

    void adoptFigure(const std::vector<yama::ivector3>& elements);

    static const int MAX_SPEED = 10;
    int speed() const { return m_speed; }

    uint32_t topNonEmptyLayer() const { return m_topNonEmptyLayer; }

private:
    // logical data
    const yama::uvector3 m_size;

    //unsigned indexInData(unsigned layer, unsigned row, unsigned col) const { return layer * m_size.x * m_size.y + row * m_size.x + col; }
    std::vector<LevelLayer*> m_levelLayers;
    uint32_t m_topNonEmptyLayer = 0;

    int m_speed = 0;

    // physical data
    // level box
    GLuint m_wireBuffer = 0;
    unsigned m_numWireVertices = 0;
    GLuint m_solidBuffer = 0;
    unsigned m_numSolidVertices = 0;

    // we wat the camera to look at 000
    // since the level takes dimensions from 000 to whd
    // we need this matrix to transform it to the appropriate place
    yama::matrix m_viewTransfrosm;
};
