// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// class for a Texture

namespace maibo
{
    class Texture
    {
    public:
        Texture();
        Texture(const std::string& name);
        ~Texture();

        bool loadFromFile(const char* filename);
        void loadFromData(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data);

        void setParameter(GLenum, GLint);

        GLenum type() const { return GL_TEXTURE_2D; }
        GLuint glHandle() const { return m_glHandle; }

        uint32_t width() const { return m_width; }
        uint32_t height() const { return m_height; }

    private:
        std::string m_name;
        GLuint m_glHandle;

        uint32_t m_width;
        uint32_t m_height;
    };
}
