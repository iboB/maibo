// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

//  RAII sentries for opengl

namespace maibo
{
    namespace gl_sentries
    {
        struct GLEnable
        {
            GLEnable(GLenum op) : operation(op)
            {
                glEnable(operation);
            }

            ~GLEnable()
            {
                glDisable(operation);
            }

            const GLenum operation;
        };

        struct GLDisable
        {
            GLDisable(GLenum op) : operation(op)
            {
                glDisable(operation);
            }

            ~GLDisable()
            {
                glEnable(operation);
            }

            const GLenum operation;
        };

        struct GLEnableAttrib
        {
            GLEnableAttrib(int i) : index(i)
            {
                glEnableVertexAttribArray(index);
            }

            ~GLEnableAttrib()
            {
                glDisableVertexAttribArray(index);
            }

            const int index;
        };

        struct GLDepthWrite
        {
            GLDepthWrite(bool write)
                : m_write(write)
            {
                glDepthMask(m_write);
            }

            ~GLDepthWrite()
            {
                glDepthMask(!m_write);
            }

            bool m_write;
        };
    }
}

#define MAIBO_GL_SENTRY(sentry, op) ::maibo::gl_sentries::sentry MAIBO_PP_CAT(sentry, __LINE__)(op)
