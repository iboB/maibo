// MaiBo
// Copyright(c) 2015-2016 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Sprite.h"
#include "maibo/Rendering/TexturePtr.h"

namespace maibo
{

// Manager sprites from a single texture
// Unlike the tileset, the sprites don't need to have the same size
class Atlas
{
public:

    void addSprite(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

private:
    ConstTexturePtr m_texture;
    std::vector<Sprite> m_sprites;
};

}
