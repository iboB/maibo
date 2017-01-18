// MaiBo
// Copyright(c) 2015-2016 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "maibo/Rendering/TexturePtr.h"
#include "Sprite.h"

namespace maibo
{

// class for a tileset
// an texture with different images, placed evely inside it
// unlike the atlast, all images within a tileset must have the same size
class Tileset
{
public:
    Tileset();

    const Texture& texture() const { return *m_texture; }
    void setTexture(ConstTexturePtr tex) { m_texture = tex; }

    // an optional border around all of the tiles (can be negative)
    void setMargin(int margin) { m_margin = margin; }

    // spacing between different tiles (can be negative)
    void setSpacing(int spacing) { m_spacing = spacing; }

    // call only one of those after setting margin and spacing (if any)
    // both will calculate the texture coordinates for all tiles
    void setTileSize(const yama::uvector2& size);
    void setTilesPerSide(const yama::uvector2& tilesPerSide);

    // tile indices are interpreted like this
    // 0 1 2 3
    // 4 5 6 7
    // ...
    // first tile is top left
    const Sprite& getTile(uint32_t tile) const { return m_tiles[tile]; }

    size_t numTiles() const { return m_tiles.size(); }

private:
    ConstTexturePtr m_texture;

    int m_margin;
    int m_spacing;

    // texture coordinates for all the tiles in the tileset
    std::vector<Sprite> m_tiles;
};

}
