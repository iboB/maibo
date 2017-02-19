// MaiBo
// Copyright(c) 2015-2016 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Tileset.h"
#include "maibo/Rendering/Texture.h"

using namespace yama;
using namespace maibo;

Tileset::Tileset()
    : m_margin(0)
    , m_spacing(0)
{
}

void Tileset::setTileSize(const yama::uvector2& size)
{
    assert(m_texture);

    const int texw = m_texture->width();
    const int texh = m_texture->height();

    // elliminate incomplete tiles at the edge if such exits
    const int limw = (texw / size.x) * size.x;
    const int limh = (texh / size.y) * size.y;

    // approximate number of tiles
    // for huge values of margin and sizing this will be a big overestimate
    size_t approxTiles = (texw / size.x) * (texh / size.y);
    m_tiles.reserve(approxTiles);

    const float pixelCenterX = (1.f/texw) / 2;
    const float pixelCenterY = (1.f/texh) / 2;

    for (int height = m_margin; height < limh - m_margin; height += m_spacing + size.y)
    {
        float top = height/float(texh) + pixelCenterY;
        float bottom = (height + size.y)/float(texh) - pixelCenterY;

        for (int width = m_margin; width < limw - m_margin; width += m_spacing + size.x)
        {
            float left = width / float(texw) + pixelCenterX;
            float right = (width + size.x) / float(texw) - pixelCenterX;

            m_tiles.resize(m_tiles.size() + 1);
            auto& sf = m_tiles.back();

            sf.tc._01 = v(left, bottom);
            sf.tc._11 = v(right, bottom);
            sf.tc._00 = v(left, top);
            sf.tc._10 = v(right, top);

            sf.texture = m_texture;
        }
    }
}

void Tileset::setTilesPerSide(const yama::uvector2& tilesPerSide)
{
    uvector2 size = vt(m_texture->width(), m_texture->height());
    size -= uint32_t(m_spacing) * (tilesPerSide - vt(1u, 1u));
    size -= uint32_t(m_margin) * tilesPerSide * 2u;
    size = div(size, tilesPerSide);
    if (size.product() == 0) return;
    setTileSize(size);
}
