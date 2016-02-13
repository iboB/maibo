// MaiBo - JoyPad
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "RenderTypes.h"

namespace joypad
{

class RenderInterface
{
public:
    virtual ~RenderInterface() {}

    struct RenderGeometryData
    {
        Vertex* vertices;
        size_t numVertices;

        uint32_t* indices;
        size_t numIndices;

        TextureHandle texture;
    };

    void renderGeometry(const RenderGeometryData& renderData) = 0;

    virtual TextureHandle loadTexture(const string& uri) = 0;
    virtual void releaseTexture(TextureHandle handle) = 0;
};

}
