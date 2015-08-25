// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "UniformColorMaterial.h"

#include <maibo/GPUProgram.h>

void UniformColorMaterial::initialize()
{
    assert(m_program);

    m_projViewLocation = m_program->getParameterByName("projView");
    assert(m_projViewLocation >= 0);
    m_modelLocation = m_program->getParameterByName("model");
    assert(m_modelLocation >= 0);
    m_colorLocation = m_program->getParameterByName("color");
    assert(m_colorLocation >= 0);

    m_vposAttribute = m_program->getAttributeLocation("v_pos");
}

void UniformColorMaterial::setProjView(const mathgp::matrix& pv)
{
    m_program->setParameter(m_projViewLocation, pv);
}

void UniformColorMaterial::setModel(const mathgp::matrix& model)
{
    m_program->setParameter(m_modelLocation, model);
}

void UniformColorMaterial::setColor(const mathgp::float4& color)
{
    m_program->setParameter(m_colorLocation, color);
}

void UniformColorMaterial::begin()
{
    m_program->use();
    glEnableVertexAttribArray(m_vposAttribute);
}

void UniformColorMaterial::end()
{
    glDisableVertexAttribArray(m_vposAttribute);
}

void UniformColorMaterial::prepareBuffer(GLuint buffer, uint32_t stride, uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(m_vposAttribute, 3, GL_FLOAT, GL_FALSE, stride, OFFSET(offset));
}