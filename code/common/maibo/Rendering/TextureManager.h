// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// class responsible for loading and storing textures

#include "TexturePtr.h"

#include "maibo/Common/ExplicitSingleton.h"

namespace maibo
{

class TextureManager : public ExplicitSingleton<TextureManager>
{
    MAIBO_DECLARE_EXPLICIT_SINGLETON(TextureManager);
public:
    TexturePtr getTexture(const std::string& name) const;
    bool hasTexture(const std::string& name) const;

    TexturePtr loadTexture(const std::string& filename, bool retain = false);
    TexturePtr loadTexture(const std::string& name, GLsizei width, GLsizei height, GLenum glFormat, const void* data, bool retain = false);

    // add an externally created texture to the manager with optional alternate name
    void addTexture(TexturePtr texture, const std::string& alternateName = std::string(), bool retain = false);

    TexturePtr errorTexture() const;

    // forcefully retain a ref to the texture
    // returns false if texture doesn't exist or is already expired
    bool retainTexture(const std::string& name);

private:

    void retainTexture(const std::string& name, TexturePtr texture);

    void createErrorTexture();

    std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;
    std::unordered_map<std::string, TexturePtr> m_retainedTextures;
    TexturePtr m_errorTexture;
};

}
