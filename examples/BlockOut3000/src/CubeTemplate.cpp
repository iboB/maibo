// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "CubeTemplate.h"

using namespace yama;
using namespace maibo;

CubeTemplate::CubeTemplate()
{
    m_wireSegments = {
        // bottom
        { v(0, 0, 0), v(1, 0, 0) },
        { v(0, 1, 0), v(1, 1, 0) },
        { v(0, 0, 0), v(0, 1, 0) },
        { v(1, 0, 0), v(1, 1, 0) },

        // tp
        { v(0, 0, 1), v(1, 0, 1) },
        { v(0, 1, 1), v(1, 1, 1) },
        { v(0, 0, 1), v(0, 1, 1) },
        { v(1, 0, 1), v(1, 1, 1) },

        // sides
        { v(0, 0, 0), v(0, 0, 1) },
        { v(1, 0, 0), v(1, 0, 1) },
        { v(1, 1, 0), v(1, 1, 1) },
        { v(0, 1, 0), v(0, 1, 1) },
    };

    glGenBuffers(1, &m_wireBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(m_wireSegments), m_wireSegments.data(), GL_STATIC_DRAW);

    m_wideWireSegments = m_wireSegments;

    for (auto& w : m_wideWireSegments)
    {
        for (auto& elem : w)
        {
            static const float e = 0.005f;
            for (auto& c : elem)
            {
                if (c == 0)
                {
                    c -= e;
                }
                else
                {
                    c += e;
                }
            }
        }
    }

    m_triangles =
    {
        //bottom
        {
            v(0, 0, 0),
            v(1, 1, 0),
            v(1, 0, 0)
        },
        {
            v(1, 1, 0),
            v(0, 0, 0),
            v(0, 1, 0)
        },

        // top
        {
            v(0, 0, 1),
            v(1, 0, 1),
            v(1, 1, 1)
        },
        {
            v(1, 1, 1),
            v(0, 1, 1),
            v(0, 0, 1)
        },

        //south
        {
            v(0, 0, 0),
            v(1, 0, 0),
            v(0, 0, 1)
        },
        {
            v(1, 0, 0),
            v(1, 0, 1),
            v(0, 0, 1)
        },

        // north
        {
            v(0, 1, 0),
            v(0, 1, 1),
            v(1, 1, 0)
        },
        {
            v(1, 1, 0),
            v(0, 1, 1),
            v(1, 1, 1)
        },

        //east
        {
            v(1, 0, 0),
            v(1, 1, 1),
            v(1, 0, 1)
        },
        {
            v(1, 1, 1),
            v(1, 0, 0),
            v(1, 1, 0)
        },

        // west
        {
            v(0, 0, 0),
            v(0, 0, 1),
            v(0, 1, 1)
        },
        {
            v(0, 1, 1),
            v(0, 1, 0),
            v(0, 0, 0)
        },
    };

    glGenBuffers(1, &m_solidBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_solidBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(m_triangles), m_triangles.data(), GL_STATIC_DRAW);

}

CubeTemplate::~CubeTemplate()
{
    glDeleteBuffers(1, &m_wireBuffer);
    glDeleteBuffers(1, &m_solidBuffer);
}


