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
#include "Preferences.h"

#include <mathgp/stdext.h>

using namespace mathgp;
using namespace std;

// ms animations last
const int ROTATION_ANIM_TIME = 200;
const int TRANSLATION_ANIM_TIME = 100;

const int FALL_TIME = 800; // milliseconds to fall a unit

Figure::Figure(const FigureTemplate& tmpl, Level& level)
    : m_template(tmpl)
    , m_level(level)
    , m_elements(m_template.elements())
    , m_tryElements(m_template.elements())
    , m_fallTimer(fallTimeForSpeed(level.speed()))
    , m_currentPosition(vc(0, 0, 0))
    , m_currentRotation(quaternion::identity())
    , m_lastPosition(vector3::zero())
    , m_targetPosition(m_currentPosition)
    , m_lastRotation(quaternion::identity())
    , m_targetRotation(m_currentRotation)
    , m_positionAnimationTimer(0)
    , m_rotationAnimationTimer(0)
{
}

namespace
{
    // matrix that rotates around a certain center
    matrix CreateOffCenterRotationMatrix(const quaternion& rot, const point3& center)
    {
        return matrix::translation(center)
            * matrix::rotation_quaternion(rot)
            * matrix::translation(-center);
    }
}

void Figure::update(uint32_t dt)
{
    if (m_isFallen) return;
    if (m_fallTimer -= dt)
    {
        if (m_fallTimer <= 0)
        {
            m_fallTimer = fallTimeForSpeed(m_level.speed()) + m_fallTimer;
            if (!tryMove(vc(0, 0, -1)))
            {
                // if figgure cannot move down, it's fallen
                m_level.adoptFigure(m_elements);
                m_isFallen = true;
                return;
            }
        }
    }

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
        * CreateOffCenterRotationMatrix(m_currentRotation, m_template.rotationCenter());
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
    assert(!m_isFallen);
    static vector3 axes[] = { vc(1, 0, 0), vc(0, 1, 0), vc(0, 0, 1) };

    quaternion rotation = quaternion::rotation_axis(axes[axis], dir * constants<float>::PI_HALF());

    // absolute transform for this rotation
    // based on it we will check if the figure can rotate like that
    matrix transform = CreateOffCenterRotationMatrix(rotation, m_template.rotationCenter());

    // this 0.5 translation is used in order to compensate for the discrepancy of the elements positions data
    // and the rotation center. The rotation center being based on cubes from 000 to 111 and the elem data
    // integer coordinates of cubes in a grid
    vector<vector3> felems(m_elements.size());
    for (size_t i = 0; i < m_elements.size(); ++i)
    {
        auto& e = m_elements[i];
        felems[i] = v(float(e.x()), float(e.y()), float(e.z())) - m_targetPosition + vector3::uniform(0.5f);
    }

    for (auto& e : felems)
    {
        e = transform_coord(e, transform);
        e += m_targetPosition - vector3::uniform(0.5f);
    }

    for (size_t i = 0; i < m_elements.size(); ++i)
    {
        auto& e = felems[i];
        m_tryElements[i] = v(int(round(e.x())), int(round(e.y())), int(round(e.z())));
    }

    if (!tryTransformWithLevel())
    {
        //desired transform cannot fit with level
        return false;
    }

    // calculate transforms for the mesh
    m_lastRotation = m_currentRotation;
    m_targetRotation = rotation * m_targetRotation;
    m_rotationAnimationTimer = ROTATION_ANIM_TIME;

    return true;
}

bool Figure::tryMove(const vector3& d, bool animate)
{
    assert(!m_isFallen);
    for (size_t i = 0; i < m_elements.size(); ++i)
    {
        m_tryElements[i] = m_elements[i] + v(int(round(d.x())), int(round(d.y())), int(round(d.z())));
    }

    if (!tryTransformWithLevel())
    {
        //desired transform cannot fit with level
        return false;
    }

    // update mesh transforms
    if (animate)
    {
        m_lastPosition = m_currentPosition;
        m_targetPosition += d;
        m_positionAnimationTimer = TRANSLATION_ANIM_TIME;
    }
    else
    {
        m_currentPosition = m_targetPosition += d;
        m_positionAnimationTimer = 0;
    }

    return true;
}

bool Figure::tryTransformWithLevel()
{
    assert(!m_isFallen);
    if (!m_level.canFitFigure(m_tryElements))
    {
        return false;
    }

    m_elements = m_tryElements;

    return true;
}

bool Figure::spawn()
{
    // Find highest point of figure
    int top = 0;
    for (const auto& e : m_elements)
    {
        if (e.z() > top)
            top = e.z();
    }

    float targetZ = float(m_level.size().z() - 1 - top);

    // Fit rotation center of figure to center of level
    auto levelCenter = vc(float(m_level.size().x()), float(m_level.size().y())) / 2;

    auto translation = levelCenter - m_template.rotationCenter().xy();

    return tryMove(vc(round(translation.x()), round(translation.y()), targetZ));
}

void Figure::startDrop()
{
    tryMove(vc(0, 0, -1), true);
    m_fallTimer = Preferences::instance().figureDropTime();
    m_isDropped = true;
}

void Figure::stopDrop()
{
    m_isDropped = false;
}

int Figure::fallTimeForSpeed(int speed) const
{
    if (m_isDropped)
    {
        return Preferences::instance().figureDropTime();
    }

    int speedMultiplier = (FALL_TIME - TRANSLATION_ANIM_TIME) / Level::MAX_SPEED;
    return TRANSLATION_ANIM_TIME + (Level::MAX_SPEED - speed) * speedMultiplier;
}
