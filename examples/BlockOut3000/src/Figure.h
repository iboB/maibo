// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// figure class

class FigureTemplate;
class Level;

class Figure
{
public:
    Figure(const FigureTemplate& tmpl, Level& level);

    void update(uint32_t dt);

    void draw() const;

    // call for the figure to set itself at a starting position in the level
    // return false if it cannot
    bool spawn();

    // all functions are "try" because they wont perform the required transform
    // unless it's possible on the level
    // they return false if the transform was unsuccessful

    // parameter is -1/1 for cw/ccw rotation
    bool tryRotateX(float dir);
    bool tryRotateY(float dir);
    bool tryRotateZ(float dir);

    // axis is 0=x, 1=y, 2=z
    bool tryRotate(int axis, float dir, bool animate = true, bool force = false);

    bool tryMove(const yama::vector3& dir, bool animate = true, bool force = false);

    bool isFallen() const { return m_isFallen; }

    // figure will try to fall faster
    void startDrop();
    void stopDrop();

private:

    // check tryElems with level
    bool checkTryElementsWithLevel() const;

    // gives the fall time for a given speed
    // if the figure dropped, it ignores the speed and just returns the drop fall time
    int fallTimeForSpeed(int speed) const;

    //////////////////////////////////////////////////
    // logical data
    // gameplay data about the figure
    // after every transformation this is recalculated
    // so that every transformation is always absolute

    const FigureTemplate& m_template;
    Level& m_level;

    std::vector<yama::ivector3> m_elements;

    // used when a transformation is performed to check if it's possible on the level
    std::vector<yama::ivector3> m_tryElements;

    // is figure fallen?
    bool m_isFallen = false;

    // is figure falling faster (user wants it to fall)
    bool m_isDropped = false;

    // fall timer
    int m_fallTimer;

    //////////////////////////////////////////////////
    // physical data
    // used to draw the figure mesh

    // model transformation matrix for drawing the figure
    // it is calculated every frame
    yama::matrix m_transform;

    // current means including animations
    // m_transform is calculated from these
    yama::point3 m_currentPosition;
    yama::quaternion m_currentRotation;

    // endpoints for animation
    yama::point3 m_lastPosition;
    yama::point3 m_targetPosition;

    yama::quaternion m_lastRotation;
    yama::quaternion m_targetRotation;

    // timers for animation, when they reach 0, current = target
    int m_positionAnimationTimer;
    int m_rotationAnimationTimer;
};