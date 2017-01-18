// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "FigureTemplate.h"

#include "CubeTemplate.h"
#include "Resources.h"

#include <maibo/Common/GLSentries.h>

using namespace std;
using namespace yama;
using namespace maibo;

FigureTemplate::FigureTemplate()
    : m_wireBuffer(0)
    , m_solidBuffer(0)
{
}

FigureTemplate::~FigureTemplate()
{
    glDeleteBuffers(1, &m_solidBuffer);
    glDeleteBuffers(1, &m_wireBuffer);
}

void FigureTemplate::preparePhysicalData()
{
    if(m_wireBuffer)
    {
        return; // already prepared
    }

    const CubeTemplate& theCube = CubeTemplate::instance();

    map<LineSegment, int, PrimitiveStrictWeakOrdering> allSegments;
    map<Triangle, int, PrimitiveStrictWeakOrdering> allTriangles;
    for(auto ielem : m_elements)
    {
        point3 elem = v(float(ielem.x), float(ielem.y), float(ielem.z));

        for(LineSegment seg : theCube.wireSegments())
        {
            seg.at(0) += elem;
            seg.at(1) += elem;

            ++allSegments[seg];
        }

        for(Triangle tri : theCube.triangles())
        {
            tri.at(0) += elem;
            tri.at(1) += elem;
            tri.at(2) += elem;

            ++allTriangles[tri];
        }
    }

    m_wireSegments.reserve(allSegments.size());
    m_triangles.reserve(allTriangles.size());

    for(auto segNum : allSegments)
    {
        if(segNum.second & 1) // odd number of encounters => we want it
        {
            m_wireSegments.push_back(segNum.first);
        }
    }

    for(auto triNum : allTriangles)
    {
        if(triNum.second == 1) // only unique ones
        {
            m_triangles.push_back(triNum.first);
        }
    }

    //if(m_name == "L")
    //{
    //    for(auto triNum : allTriangles)
    //    {
    //        auto tri = triNum.first;
    //        sort(tri.begin(), tri.end(), yama::strict_ordering());
    //        for(auto point : tri)
    //        {
    //            cout << point << ", ";
    //        }
    //        cout << triNum.second;
    //        cout << endl;
    //    }
    //}

    // create buffers
    glGenBuffers(1, &m_wireBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(m_wireSegments), m_wireSegments.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_solidBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_solidBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(m_triangles), m_triangles.data(), GL_STATIC_DRAW);
}


void FigureTemplate::draw(const vector4& solidColor, const vector4& wireColor) const
{
    UniformColorMaterial& m = Resources::instance().uniformColorMaterial;

    MAIBO_GL_SENTRY(GLDepthWrite, false);

    m.setColor(wireColor);
    m.prepareBuffer(m_wireBuffer, sizeof(vector3), 0);
    glDrawArrays(GL_LINES, 0, uint32_t(m_wireSegments.size() * 2));

    MAIBO_GL_SENTRY(GLEnable, GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m.setColor(solidColor);
    m.prepareBuffer(m_solidBuffer, sizeof(vector3), 0);

    glCullFace(GL_FRONT);
    glDrawArrays(GL_TRIANGLES, 0, uint32_t(m_triangles.size() * 3));
    glCullFace(GL_BACK);
    glDrawArrays(GL_TRIANGLES, 0, uint32_t(m_triangles.size() * 3));
}
