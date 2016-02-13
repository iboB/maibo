// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// shared pointers for GPUProgram

namespace maibo
{
    class GPUProgram;

    typedef std::shared_ptr<GPUProgram> GPUProgramPtr;
    typedef std::shared_ptr<const GPUProgram> ConstGPUProgramPtr;
}
