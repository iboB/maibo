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

    void debugDraw() const;

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    const mathgp::point3& rotationCenterX() const { return m_rotationCenterX; }
    void setRotationCenterX(const mathgp::point3& x) { m_rotationCenterX = x; }

    const mathgp::point3& rotationCenterY() const { return m_rotationCenterY; }
    void setRotationCenterY(const mathgp::point3& y) { m_rotationCenterY = y; }

    const mathgp::point3& rotationCenterZ() const { return m_rotationCenterZ; }
    void setRotationCenterZ(const mathgp::point3& z) { m_rotationCenterZ = z; }

    void addElement(const mathgp::ivector3& elem) { m_elements.push_back(elem); }

private:

    //////////////////////////////////////
    // logical data
    std::string m_name;
    std::vector<mathgp::ivector3> m_elements;

    mathgp::point3 m_rotationCenterX;
    mathgp::point3 m_rotationCenterY;
    mathgp::point3 m_rotationCenterZ;

    //////////////////////////////////////
    // rendering (physical data)
    std::vector<LineSegment> m_wireSegments;
    std::vector<Triangle> m_triangles;
    GLuint m_wireBuffer;
    GLuint m_solidBuffer;
};
