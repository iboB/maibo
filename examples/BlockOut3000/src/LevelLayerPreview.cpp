// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LevelLayerPreview.h"

#include "Primitives.h"
#include "Resources.h"
#include "Preferences.h"

#include <maibo/lib/memory.h>
#include <maibo/lib/GLSentries.h>

using namespace std;
using namespace mathgp;
using namespace maibo;

LevelLayerPreview::LevelLayerPreview()
{
    glGenBuffers(1, &m_solidBuffer);
    glGenBuffers(1, &m_wireBuffer);

    std::vector<Triangle> solidTriangles = {
        // front
        {
            vc(0, 0, 0),
            vc(1, 0, 0),
            vc(1, 1, 0),
        },
        {
            vc(1, 1, 0),
            vc(0, 1, 0),
            vc(0, 0, 0),
        },
        {
            vc(1, 0, 0),
            vc(2, 0, 0),
            vc(2, 1, 0),
        },
        {
            vc(2, 1, 0),
            vc(1, 1, 0),
            vc(1, 0, 0),
        },


        // side
        {
            vc(2, 0, 0),
            vc(2.5f, 0.5f, 0),
            vc(2.5f, 1.5f, 0),
        },
        {
            vc(2.5f, 1.5f, 0),
            vc(2, 1, 0),
            vc(2, 0, 0),
        },
        {
            vc(2.5f, 0.5f, 0),
            vc(3, 1, 0),
            vc(3, 2, 0),
        },
        {
            vc(3, 2, 0),
            vc(2.5f, 1.5f, 0),
            vc(2.5f, 0.5f, 0),
        },

        // top
        {
            vc(0, 1, 0),
            vc(1, 1, 0),
            vc(1.5f, 1.5f, 0),
        },
        {
            vc(1.5f, 1.5f, 0),
            vc(0.5f, 1.5f, 0),
            vc(0, 1, 0),
        },
        {
            vc(1, 1, 0),
            vc(2, 1, 0),
            vc(2.5f, 1.5f, 0),
        },
        {
            vc(2.5f, 1.5f, 0),
            vc(1.5f, 1.5f, 0),
            vc(1, 1, 0),
        },

        {
            vc(0.5f, 1.5, 0),
            vc(1.5f, 1.5, 0),
            vc(2, 2, 0),
        },
        {
            vc(2, 2, 0),
            vc(1, 2, 0),
            vc(0.5f, 1.5f, 0),
        },
        {
            vc(1.5f, 1.5f, 0),
            vc(2.5f, 1.5f, 0),
            vc(3, 2, 0),
        },
        {
            vc(3, 2, 0),
            vc(2, 2, 0),
            vc(1.5f, 1.5f, 0),
        },

    };

    std::vector<LineSegment> wireSegments = {
        // horizontal
        {
            vc(0, 0, 0),
            vc(2, 0, 0)
        },
        {
            vc(0, 1, 0),
            vc(2, 1, 0)
        },
        {
            vc(0.5f, 1.5f, 0),
            vc(2.5f, 1.5f, 0)
        },
        {
            vc(1, 2, 0),
            vc(3, 2, 0)
        },

        // vertical
        {
            vc(0, 0, 0),
            vc(0, 1, 0)
        },
        {
            vc(1, 0, 0),
            vc(1, 1, 0)
        },
        {
            vc(2, 0, 0),
            vc(2, 1, 0)
        },
        {
            vc(2.5f, 0.5f, 0),
            vc(2.5f, 1.5f, 0)
        },
        {
            vc(3, 1, 0),
            vc(3, 2, 0)
        },

        // slanted
        {
            vc(0, 1, 0),
            vc(1, 2, 0)
        },
        {
            vc(1, 1, 0),
            vc(2, 2, 0)
        },
        {
            vc(2, 1, 0),
            vc(3, 2, 0)
        },
        {
            vc(2, 0, 0),
            vc(3, 1, 0)
        },
    };

    m_numSolidVertices = uint32_t(solidTriangles.size()) * 3;
    m_numWireVertices = uint32_t(wireSegments.size()) * 2;

    glBindBuffer(GL_ARRAY_BUFFER, m_solidBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(solidTriangles), solidTriangles.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(wireSegments), wireSegments.data(), GL_STATIC_DRAW);
}

LevelLayerPreview::~LevelLayerPreview()
{
    glDeleteBuffers(1, &m_solidBuffer);
    glDeleteBuffers(1, &m_wireBuffer);
}

void LevelLayerPreview::draw(size_t numLayers)
{
    MAIBO_GL_SENTRY(GLDepthWrite, false);

    UniformColorMaterial& m = Resources::instance().uniformColorMaterial;
    m.setProjView(matrix::ortho_rh(0, 3.1f, 0, 30, 0, 1));

    for (uint32_t i = 0; i < numLayers; ++i)
    {
        m.setModel(matrix::translation(0, 1.8f * i, 0));
        m.setColor(Preferences::instance().layerColor(i));
        m.prepareBuffer(m_solidBuffer, sizeof(vector3), 0);
        glDrawArrays(GL_TRIANGLES, 0, m_numSolidVertices);

        m.setColor(vc(0.7f, 0.7f, 0.7f, 1));
        m.prepareBuffer(m_wireBuffer, sizeof(vector3), 0);
        glDrawArrays(GL_LINES, 0, m_numWireVertices);
    }
}