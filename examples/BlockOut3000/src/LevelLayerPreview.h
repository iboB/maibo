// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Preview of current stacked layers

class LevelLayerPreview
{
public:
    LevelLayerPreview();
    ~LevelLayerPreview();

    void draw(size_t numLayers);

private:
    // physical data

    // physical data
    GLuint m_wireBuffer = 0;
    unsigned m_numWireVertices = 0;
    GLuint m_solidBuffer = 0;
    unsigned m_numSolidVertices = 0;
};
