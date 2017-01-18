// MaiBo - JoyPad
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

namespace maibo
{
namespace joypad
{

typedef intptr_t TextureHandle;

struct Vertex
{
    yama::point3 position;
    yama::point2 texCoord;
    yama::point4 color;
};

}
}
