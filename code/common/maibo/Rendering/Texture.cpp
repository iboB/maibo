// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Texture.h"

//#include "ResourceConstants.h"

using namespace std;
using namespace maibo;

Texture::Texture()
    : Texture("<unnamed texture>")
{
}

Texture::Texture(const std::string& name)
    : m_width(0)
    , m_height(0)
    , m_name(name)
{
    glGenTextures(1, &m_glHandle);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_glHandle);
}

bool Texture::loadFromFile(const char* filename)
{
    SDL_Surface* image = IMG_Load(filename);
    if (!image)
    {
        const char* err = IMG_GetError();
        cerr << "couldn't load " << filename << ": " << err << endl;
        return false;
    }

    GLenum glFormat;

    auto sdlFormat = image->format->format;

    switch (sdlFormat)
    {
    case SDL_PIXELFORMAT_RGB24:
    case SDL_PIXELFORMAT_RGB888:
        glFormat = GL_RGB;
        break;
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_BGRA8888:
    case SDL_PIXELFORMAT_ARGB8888:
    case SDL_PIXELFORMAT_ABGR8888: // png-s come with this???
        glFormat = GL_RGBA;
        break;
    default:
    {
        glFormat = GL_RGB;

        m_width = image->w;
        m_height = image->h;

        cerr << "unsupported texture format for " << filename << endl;
        return false;
    }
    break;
    }

    loadFromData(image->w, image->h, glFormat, image->pixels);

    SDL_FreeSurface(image);

    return true;
}

void Texture::loadFromData(GLsizei width, GLsizei height, GLenum format, const void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_glHandle);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    m_format = format;
    m_width = width;
    m_height = height;
}

void Texture::setParameter(GLenum param, GLint value)
{
    glTexParameteri(GL_TEXTURE_2D, param, value);
}
