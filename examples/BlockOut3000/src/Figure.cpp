// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Figure.h"
#include "Level.h"

#include "FigureTemplate.h"

#include "Resources.h"

using namespace mathgp;

// ms animations last
const int ROTATION_ANIM_TIME = 200;
const int TRANSLATION_ANIM_TIME = 100;

Figure::Figure(const FigureTemplate& tmpl, Level& level)
    : m_template(tmpl)
    , m_level(level)
    , m_currentPosition(vc(0, 0, 8))
    , m_currentRotation(quaternion::identity())
    , m_lastPosition(vector3::zero())
    , m_targetPosition(m_currentPosition)
    , m_lastRotation(quaternion::identity())
    , m_targetRotation(m_currentRotation)
    , m_positionAnimationTimer(0)
    , m_rotationAnimationTimer(0)
{

}

void Figure::update(uint32_t dt)
{
    if (m_positionAnimationTimer)
    {
        m_positionAnimationTimer -= dt;
        if (m_positionAnimationTimer < 0)
            m_positionAnimationTimer = 0;

        float f = 1 - float(m_positionAnimationTimer) / TRANSLATION_ANIM_TIME;
        m_currentPosition = lerp(m_lastPosition, m_targetPosition, f);
    }

    if (m_rotationAnimationTimer)
    {
        m_rotationAnimationTimer -= dt;
        if (m_rotationAnimationTimer < 0)
            m_rotationAnimationTimer = 0;

        float f = 1 - float(m_rotationAnimationTimer) / ROTATION_ANIM_TIME;
        m_currentRotation = slerp(m_lastRotation, m_targetRotation, f);
    }

    m_transform = matrix::translation(m_currentPosition) 
        * matrix::translation(m_template.rotationCenter())
        * matrix::rotation_quaternion(m_currentRotation)
        * matrix::translation(-m_template.rotationCenter());
}

void Figure::draw() const
{
    UniformColorMaterial& m = Resources::instance().uniformColorMaterial;

    m.setModel(m_transform);

    m_template.draw(vc(0.9f, 0.5f, 0.5f, 0.1f), vc(1, 1, 1, 1));
}

bool Figure::tryRotateX(float dir)
{
    return tryRotate(0, dir);
}

bool Figure::tryRotateY(float dir)
{
    return tryRotate(1, dir);
}

bool Figure::tryRotateZ(float dir)
{
    return tryRotate(2, dir);
}

bool Figure::tryRotate(int axis, float dir)
{
    static vector3 axes[] = { vc(1, 0, 0), vc(0, 1, 0), vc(0, 0, 1) };

    quaternion rotation = quaternion::rotation_axis(axes[axis], dir * constants<float>::PI_HALF());

    // absolute transform for this rotation
    // based on it we will check if the figure can rotate like that
    matrix transform = matrix::rotation_quaternion(rotation);

    // calculate transforms for the mesh
    m_lastRotation = m_currentRotation;
    m_targetRotation = rotation * m_targetRotation;
    m_rotationAnimationTimer = ROTATION_ANIM_TIME;

    return true;
}

bool Figure::tryMove(const vector3& direction)
{
    
    // update mesh transforms
    m_lastPosition = m_currentPosition;
    m_targetPosition = m_currentPosition + direction;
    m_positionAnimationTimer = TRANSLATION_ANIM_TIME;

    return true;
}
