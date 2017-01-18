// MaiBo
// Copyright(c) 2015-2016 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "maibo/Rendering/TexturePtr.h"

namespace maibo
{

// class for a sprite frame
// it could come from a tileset or atlas
// it is basically a wrapper for a set of texture coordiates
// and a texture reference
// it has publically accesible members
//
// Should be a front face with indices:
// 0, 1, 2,
// 2, 1, 3,
struct Sprite
{
    // contains texture coordinates for the four sides of a frame
    struct TexCoord
    {
        // ccw order coincinding with vertex coordinates 00 10 01 11
        yama::vector2 _01; // with texcoords equivalent to 01 on a normal texture
        yama::vector2 _11;
        yama::vector2 _00;
        yama::vector2 _10;

        // initializes the texturecoordinates to cover a full texture
        void init01();

        // mirroring ops
        void flipHorizontally();
        void flipVertically();
        void flipDiagonally();
    }
    tc;

    ConstTexturePtr texture;
};

}
