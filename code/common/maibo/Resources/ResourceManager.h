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
#include "ResourceFuture.h"

#include "maibo/Rendering/ShaderPtr.h"
#include "maibo/Rendering/GPUProgramPtr.h"
#include "maibo/Rendering/ShaderType.h"

namespace maibo
{
    class ResourceManager : public ImplicitSingleton<ResourceManager>
    {
    public:
        // Check if a file exists on the local FS.
        // It should either be preloaded or obtained by a wget. This function won't try wgetting it
        // Warning: This function returns an error code! 0 is a success.
        int checkFileExists(const std::string& path);

        // Asynchronously obtain a file
        // Returns an error code
        // * Desktop OS: basically checks if the file exists
        // * Emscripten: First checks if the file exists on the local FS
        //   If it doesn't, try wgetting it
        ResourceFuturePtr<int> getFileAsync(const std::string& path);

        // Reads a file from the fs and stores it to the vector
        // The manager does NOT check if the file has been read or obtained before
        // This means that the read op will happen every time when you request it
        std::vector<char> readFile(const std::string& path); // no alsoGetFile, since we don't support a synchronous get
        ResourceFuturePtr<std::vector<char>> readFileAsync(const std::string& path, bool alsoGetFile = false);

        // Loads a shader from a file
        ResourceFuturePtr<ShaderPtr> loadShaderAsync(const std::string& path, ShaderType::Type type, bool alsoGetFile = false);

        // Loads a GPU program from two shader types
        ResourceFuturePtr<GPUProgramPtr> loadGPUProgramAsync(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool alsoGetFiles = false);
    };
}