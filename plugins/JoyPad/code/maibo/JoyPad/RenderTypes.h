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
    mathgp::point3 position;
    mathgp::point2 texCoord;
    mathgp::point4 color;
};

}
}
