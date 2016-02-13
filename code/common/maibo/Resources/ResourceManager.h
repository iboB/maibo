// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Resource manager singleton

#include "maibo/Common/ImplicitSingleton.h"
#include "maibo/Common/ResourceFuture.h"

#include "maibo/Rendering/ShaderPtr.h"
#include "maibo/Rendering/GPUProgramPtr.h"
#include "maibo/Rendering/ShaderType.h"

namespace maibo
{
    class ResourceManager : public ImplicitSingleton<ResourceManager>
    {
    public:


        // Loads a shader from a file
        ResourceFuturePtr<ShaderPtr> loadShaderAsync(const std::string& path, ShaderType::Type type, bool alsoGetFile = false);

        // Loads a GPU program from two shader types
        ResourceFuturePtr<GPUProgramPtr> loadGPUProgramAsync(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool alsoGetFiles = false);
    };
}