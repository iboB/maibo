// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/GPUProgramPtr.h>

// simple class for a Uniform color material

class UniformColorMaterial
{
public:

    void setPVM(const mathgp::matrix& pvm);
    void setColor(const mathgp::float4& color);

    // call these before and after rendering with this material
    void begin();
    void end();

    void prepareBuffer(GLuint buffer, uint32_t stride, uint32_t offset);

private:
    friend class LoadAllState;

    void initialize();

    maibo::GPUProgramPtr m_program;

    GLint m_pvmLocation = -1;
    GLint m_colorLocation = -1;

    uint32_t m_vposAttribute = 0;
};
