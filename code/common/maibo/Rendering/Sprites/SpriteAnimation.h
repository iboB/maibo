// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Sprite.h"

namespace maibo
{

class Tileset;

// class for sprite animation
class SpriteAnimation
{
public:
    SpriteAnimation();

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    void setMsPerFrame(uint32_t msPerFrame) { m_msPerFrame = msPerFrame; }
    uint32_t msPerFrame() const { return m_msPerFrame; }

    // load the animation from tileset when the frames are continuous tiles
    void loadContinuousFramesFromTileset(const Tileset& tileset, uint32_t firstFrame, uint32_t lastFrame);

    // flips the frames horizontally
    void flipFrames();

    void play(bool loop);

    // try to morph into the target animation
    // morph means to try to set the same frame and same frame time as the target
    void morphInto(const SpriteAnimation& target);

    enum StopType
    {
        WaitTillEnd,
        Force,
    };

    // if wait till end it true the animation will stop only after it has reached
    // its last frame
    void stop(StopType stopType);

    bool done() const { return !m_playing; }
    bool loop() const { return m_loop; }

    // set current frame to 0
    // doesn't stop the animation if it's playing
    void reset();

    void update(int dt);

    const Sprite& getCurrentFrame() const { return m_frames[m_currentFrameIndex]; }

private:
    std::string m_name;

    int m_currentFrameTime; // how many milliseconds at current frame
    uint32_t m_msPerFrame; // milliseconds per frame

    bool m_playing; // is it playing
    bool m_loop; // does it loop after finishing

    size_t m_currentFrameIndex;
    std::vector<Sprite> m_frames;
};

}
