#include "Level.h"

using namespace mathgp;
using namespace std;

Level::Level(const uvector3& size)
    : m_size(size)
{
}

void Level::createBuffers()
{
    float w = float(m_size.x());
    float h = float(m_size.y());
    float d = float(m_size.z());

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
