// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "CubeTemplate.h"

using namespace mathgp;
using namespace maibo;

CubeTemplate::CubeTemplate()
{
    m_wireSegments = {
        // bottom
        { vc(0, 0, 0), vc(1, 0, 0) },
        { vc(0, 1, 0), vc(1, 1, 0) },
        { vc(0, 0, 0), vc(0, 1, 0) },
        { vc(1, 0, 0), vc(1, 1, 0) },

        // tp
        { vc(0, 0, 1), vc(1, 0, 1) },
        { vc(0, 1, 1), vc(1, 1, 1) },
        { vc(0, 0, 1), vc(0, 1, 1) },
        { vc(1, 0, 1), vc(1, 1, 1) },

        // sides
        { vc(0, 0, 0), vc(0, 0, 1) },
        { vc(1, 0, 0), vc(1, 0, 1) },
        { vc(1, 1, 0), vc(1, 1, 1) },
        { vc(0, 1, 0), vc(0, 1, 1) },
    };


    glGenBuffers(1, &m_wireBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(m_wireSegments), m_wireSegments.data(), GL_STATIC_DRAW);

    m_triangles =
    {
        //bottom
        {
            vc(0, 0, 0),
            vc(1, 1, 0),
            vc(1, 0, 0)
        },
        {
            vc(1, 1, 0),
            vc(0, 0, 0),
            vc(0, 1, 0)
        },

        // top
        {
            vc(0, 0, 1),
            vc(1, 0, 1),
            vc(1, 1, 1)
        },
        {
            vc(1, 1, 1),
            vc(0, 1, 1),
            vc(0, 0, 1)
        },

        //south
        {
            vc(0, 0, 0),
            vc(1, 0, 0),
            vc(0, 0, 1)
        },
        {
            vc(1, 0, 0),
            vc(1, 0, 1),
            vc(0, 0, 1)
        },

        // north
        {
            vc(0, 1, 0),
            vc(0, 1, 1),
            vc(1, 1, 0)
        },
        {
            vc(1, 1, 0),
            vc(0, 1, 1),
            vc(1, 1, 1)
        },

        //east
        {
            vc(1, 0, 0),
            vc(1, 1, 1),
            vc(1, 0, 1)
        },
        {
            vc(1, 1, 1),
            vc(1, 0, 0),
            vc(1, 1, 0)
        },

        // west
        {
            vc(0, 0, 0),
            vc(0, 0, 1),
            vc(0, 1, 1)
        },
        {
            vc(0, 1, 1),
            vc(0, 1, 0),
            vc(0, 0, 0)
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


