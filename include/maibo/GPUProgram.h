// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "ShaderPtr.h"

// class for a GPU program (effect)

namespace maibo
{
    class Texture;

    class GPUProgram
    {
    public:
        GPUProgram();
        GPUProgram(const std::string& name); // name is used for debugging purposes
        ~GPUProgram();

        void attachShader(ConstShaderPtr shader);

        bool link();
        void use();

        void resetUniforms(); // call before adding a new set uniforms to the program

        GLint getParameterByName(const char* name) const;

        // returns the index where the attrbute was bound
        uint32_t bindCustomAttribute(const char* name); // bind custom attribute

        uint32_t numBoundAttribs() const { return m_boundAttribPtr; }

        void setParameter(GLint handle, float value);
        void setParameter(GLint handle, const mathgp::vector2& vec);
        void setParameter(GLint handle, const mathgp::vector3& vec);
        void setParameter(GLint handle, const mathgp::vector4& vec);
        void setParameter(GLint handle, const mathgp::matrix& matrix);

        void setParameter(GLint handle, const Texture& tex);

    private:
        const std::string m_name;

        GLuint m_glHandle;

        std::vector<ConstShaderPtr> m_shaders;
        uint32_t m_boundAttribPtr;

        size_t m_boundTextures;
    };
}