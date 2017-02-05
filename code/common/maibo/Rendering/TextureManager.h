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
public:
    MAIBO_DECLARE_EXPLICIT_SINGLETON(TextureManager);

    TexturePtr getTexture(const std::string& name) const;
    bool hasTexture(const std::string& name) const;

    TexturePtr loadTexture(const std::string& filename);
    TexturePtr loadTexture(const std::string& name, GLsizei width, GLsizei height, GLenum glFormat, const void* data);

    TexturePtr errorTexture() const;

private:

    void createErrorTexture();

    std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;
    TexturePtr m_errorTexture;
};

}
