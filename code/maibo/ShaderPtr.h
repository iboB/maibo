// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// shared pointers for Shader

namespace maibo
{
    class Shader;

    typedef std::shared_ptr<Shader> ShaderPtr;
    typedef std::shared_ptr<const Shader> ConstShaderPtr;
}
