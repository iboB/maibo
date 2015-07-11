// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "ShaderType.h"

// class for a shader

namespace maibo
{
    class Shader
    {
    public:
        Shader(ShaderType::Type t);
        Shader(ShaderType::Type t, const std::string& name);
        ~Shader();

        // buffer is simply the shader text
        bool load(const char* buffer);

        ShaderType::Type type() const { return m_type; }

    private:
        friend class GPUProgram;

        ShaderType::Type m_type;
        std::string m_name;

        GLuint m_glHandle;
        GLenum m_glType;
    };
}
