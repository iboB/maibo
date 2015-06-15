// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Resource manager singleton

#include "ImplicitSingleton.h"
#include "ResourceFuture.h"

namespace maibo
{
    class ResourceManager : public ImplicitSingleton<ResourceManager>
    {
    public:
        // Ensure the file exists on the local FS
        // Returns an error code
        // * Desktop OS: basically checks if the file exists
        // * Emscripten: checks if the file exists, and if not, wget-s it from the server
        int GetFile(const std::string& path);
        ResourceFuture<int> GetFileAsync(const std::string& path);

        // Reads a file from the fs and stores it to the vector
        // The manager does NOT check if the file has been read or obtained before
        std::vector<char> ReadFile(const std::string& path);
        ResourceFuture<std::vector<char>> ReadFileAsync(const std::string& path);

    };
}