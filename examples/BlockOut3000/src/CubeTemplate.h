// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// a renderable cube template

#include "Primitives.h"
#include <maibo/Common/ExplicitSingleton.h>

class CubeTemplate : public maibo::ExplicitSingleton<CubeTemplate>
{
public:
    const std::vector<LineSegment>& wireSegments() const { return m_wireSegments; }
    const std::vector<LineSegment>& wideWireSegments() const { return m_wideWireSegments; }
    const std::vector<Triangle>& triangles() const { return m_triangles; }

    GLuint solidBuffer() const { return m_solidBuffer; }

private:
    MAIBO_DECLARE_EXPLICIT_SINGLETON(CubeTemplate);

    GLuint m_wireBuffer;
    std::vector<LineSegment> m_wireSegments;

    // a helper array of segments which are a tiny bit bigger in order to draw them
    // with depth testing enabled while they still remain visible
    std::vector<LineSegment> m_wideWireSegments;

    GLuint m_solidBuffer;
    std::vector<Triangle> m_triangles;
};