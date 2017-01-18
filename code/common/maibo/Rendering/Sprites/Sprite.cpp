// MaiBo
// Copyright(c) 2015-2016 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Sprite.h"

using namespace std;
using namespace yama;
using namespace maibo;

void Sprite::TexCoord::init01()
{
    _01 = v(0, 1);
    _11 = v(1, 1);
    _00 = v(0, 0);
    _10 = v(1, 0);
}

void Sprite::TexCoord::flipHorizontally()
{
    swap(_00, _10);
    swap(_01, _11);
}

void Sprite::TexCoord::flipVertically()
{
    swap(_00, _01);
    swap(_10, _11);
}

void Sprite::TexCoord::flipDiagonally()
{
    swap(_00, _11);
}
