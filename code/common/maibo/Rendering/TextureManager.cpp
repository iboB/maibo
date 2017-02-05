// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "TextureManager.h"
#include "Texture.h"

using namespace std;

namespace maibo
{

TextureManager::TextureManager()
{
    createErrorTexture();
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::getTexture(const std::string& name) const
{
    auto f = m_textures.find(name);
    if (f == m_textures.end() || f->second.expired())
    {
        return m_errorTexture;
    }
    else
    {
        return f->second.lock();
    }

}

bool TextureManager::hasTexture(const std::string& name) const
{
    auto f = m_textures.find(name);
    return f == m_textures.end();
}

TexturePtr TextureManager::loadTexture(const std::string& filename)
{
    auto texture = make_shared<Texture>(filename);
    if (texture->loadFromFile(filename.c_str()))
    {
        m_textures[filename] = texture;
    }
    else
    {
        texture = m_errorTexture;
    }

    return texture;
}

TexturePtr TextureManager::loadTexture(const std::string& name, GLsizei width, GLsizei height, GLenum glFormat, const void* data)
{
    auto texture = make_shared<Texture>(name);
    texture->loadFromData(width, height, glFormat, data);
    m_textures[name] = texture;
    return texture;
}

TexturePtr TextureManager::errorTexture() const
{
    return m_errorTexture;
}

void TextureManager::createErrorTexture()
{
    m_errorTexture = make_shared<Texture>("<error>");
    uint32_t pixel = 0xFF0000FF;
    m_errorTexture->loadFromData(1, 1, GL_RGBA, &pixel);
    m_errorTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_errorTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_errorTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_errorTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

}