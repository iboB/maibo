// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// a tetris figure template. Has no actual position of level info

#include "Primitives.h"

class FigureTemplate
{
public:
    FigureTemplate();
    ~FigureTemplate();

    void preparePhysicalData();

    void draw(const yama::vector4& solidColor, const yama::vector4& wireColor) const;

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    const yama::point3& rotationCenter() const { return m_rotationCenter; }
    void setRotationCenter(const yama::point3& c) { m_rotationCenter = c; }

    void addElement(const yama::ivector3& elem) { m_elements.push_back(elem); }

    const std::vector<yama::ivector3>& elements() const { return m_elements; }
private:

    //////////////////////////////////////
    // logical data
    std::string m_name;
    std::vector<yama::ivector3> m_elements;

    yama::point3 m_rotationCenter;

    //////////////////////////////////////
    // rendering (physical data)
    std::vector<LineSegment> m_wireSegments;
    std::vector<Triangle> m_triangles;
    GLuint m_wireBuffer;
    GLuint m_solidBuffer;
};
