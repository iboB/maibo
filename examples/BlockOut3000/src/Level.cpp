// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Level.h"

#include "Resources.h"

#include <maibo/lib/GLSentries.h>

using namespace mathgp;
using namespace std;

Level::Level(const uvector3& size)
    : m_size(size)
    , m_levelData(new char[size.x()*size.y()*size.z()])
{
}

Level::~Level()
{
    delete[] m_levelData;
}

void Level::draw() const
{
    MAIBO_GL_SENTRY(GLDisable, GL_DEPTH_TEST);

    UniformColorMaterial& m = Resources::instance().uniformColorMaterial;

    m.setModel(matrix::identity());

    m.setColor(vc(0.066f, 0.066f, 0.15f, 1));
    m.prepareBuffer(m_solidBuffer, sizeof(vector3), 0);
    glDrawArrays(GL_TRIANGLES, 0, m_numSolidVertices);

    m.setColor(vc(0.13f, 0.30f, 0.15f, 1));
    m.prepareBuffer(m_wireBuffer, sizeof(vector3), 0);
    glDrawArrays(GL_LINES, 0, m_numWireVertices);
}

void Level::createBuffers()
{
    float w = float(m_size.x());
    float h = float(m_size.y());
    float d = float(m_size.z());

    // biggest size of w and h tobe trunkated to 1
    float dim = max(w, h)/2;

    m_viewTransfrosm = matrix::scaling_uniform(1/dim) * matrix::translation(-w / 2, -h / 2, -d);

    point3 solidVertices[] =
    {
        //bottom
        vc(0, 0, 0),
        vc(w, 0, 0),
        vc(w, h, 0),

        vc(w, h, 0),
        vc(0, h, 0),
        vc(0, 0, 0),

        //south
        vc(0, 0, 0),
        vc(0, 0, d),
        vc(w, 0, 0),

        vc(w, 0, 0),
        vc(0, 0, d),
        vc(w, 0, d),

        //east
        vc(w, 0, 0),
        vc(w, 0, d),
        vc(w, h, d),

        vc(w, h, d),
        vc(w, h, 0),
        vc(w, 0, 0),

        // north
        vc(0, h, 0),
        vc(w, h, 0),
        vc(w, h, d),

        vc(w, h, d),
        vc(0, h, d),
        vc(0, h, 0),

        // west
        vc(0, 0, 0),
        vc(0, h, 0),
        vc(0, 0, d),

        vc(0, 0, d),
        vc(0, h, 0),
        vc(0, h, d),

    };

    glGenBuffers(1, &m_solidBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_solidBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(solidVertices), solidVertices, GL_STATIC_DRAW);
    m_numSolidVertices = _countof(solidVertices);

    vector<point3> wireVertices;
    wireVertices.reserve(2 * 3 * (m_size.x() + 1) + 2 * 3 * (m_size.y() + 1) + 2 * 4 * (m_size.z() + 1));

    // concentric squares
    for (unsigned i = 0; i<m_size.z() + 1; ++i)
    {
        float d = float(i);
        wireVertices.insert(wireVertices.end(), {
            vc(0, 0, d),
            vc(w, 0, d), vc(w, 0, d),
            vc(w, h, d), vc(w, h, d),
            vc(0, h, d), vc(0, h, d),
            vc(0, 0, d) 
        });
    }

    // vertical U
    for (unsigned i = 0; i<m_size.x() + 1; ++i)
    {
        float w = float(i);
        wireVertices.insert(wireVertices.end(), {
            vc(w, 0, d),
            vc(w, 0, 0), vc(w, 0, 0),
            vc(w, h, 0), vc(w, h, 0),
            vc(w, h, d)
        });
    }

    // horizontal U
    for (unsigned i = 0; i<m_size.y() + 1; ++i)
    {
        float h = float(i);
        wireVertices.insert(wireVertices.end(), {
            vc(0, h, d),
            vc(0, h, 0), vc(0, h, 0),
            vc(w, h, 0), vc(w, h, 0),
            vc(w, h, d)
        });
    }

    glGenBuffers(1, &m_wireBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
    glBufferData(GL_ARRAY_BUFFER, wireVertices.size()*sizeof(point3), wireVertices.data(), GL_STATIC_DRAW);
    m_numWireVertices = unsigned(wireVertices.size());
}
