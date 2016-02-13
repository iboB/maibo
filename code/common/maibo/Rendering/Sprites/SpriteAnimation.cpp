// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "SpriteAnimation.h"

#include "Tileset.h"

using namespace maibo;

SpriteAnimation::SpriteAnimation()
    : m_currentFrameTime(0)
    , m_msPerFrame(250) // 4 fps
    , m_playing(false)
    , m_loop(false)
    , m_currentFrameIndex(0)
{
}

void SpriteAnimation::loadContinuousFramesFromTileset(const Tileset& tileset, uint32_t firstFrame, uint32_t lastFrame)
{
    m_frames.reserve(lastFrame - firstFrame + 1);
    for (uint32_t i = firstFrame; i <= lastFrame; ++i)
    {
        m_frames.emplace_back(tileset.getTile(i));
    }
}

void SpriteAnimation::flipFrames()
{
    for (auto& frame : m_frames)
    {
        frame.tc.flipHorizontally();
    }
}

void SpriteAnimation::reset()
{
    m_currentFrameIndex = 0;
    m_currentFrameTime = 0;
}

void SpriteAnimation::play(bool loop)
{
    m_playing = true;
    m_loop = loop;
}

void SpriteAnimation::morphInto(const SpriteAnimation& target)
{
    m_playing = target.m_playing;
    m_loop = target.m_loop;

    m_currentFrameTime = target.m_currentFrameTime;

    m_currentFrameIndex = target.m_currentFrameIndex;

    if (m_frames.size() <= target.m_currentFrameIndex)
    {
        // not enough frames
        // fall back to the last one
        m_currentFrameIndex = m_frames.size() - 1;
    }
}

void SpriteAnimation::stop(SpriteAnimation::StopType stopType)
{
    if (stopType == WaitTillEnd)
    {
        m_loop = false;
    }
    else
    {
        m_playing = false;
    }
}

void SpriteAnimation::update(int dt)
{
    if (!m_playing)
        return;

    m_currentFrameTime += dt;

    while (m_currentFrameTime > int(m_msPerFrame))
    {
        m_currentFrameTime -= m_msPerFrame;

        if (m_loop)
        {
            m_currentFrameIndex = (m_currentFrameIndex + 1) % m_frames.size();
        }
        else
        {
            if (m_currentFrameIndex == m_frames.size() - 1)
            {
                // not looping. finished. stop.
                m_playing = false;
            }
            else
            {
                ++m_currentFrameIndex;
            }
        }
    }
}
