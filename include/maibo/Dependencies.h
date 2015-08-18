// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// MaiBo dependencies
// All of these should be included before any MaiBo file
// The best choice is to include those in the library's precompiled header

// sdl is not situated in a specific folder for msvc
// who knows why...
#if defined(_MSC_VER)
#   include <SDL.h>
#else
#   include <SDL2/SDL.h>
#endif

#include <SDL_image.h>

#if defined(_WIN32)
#    include <GL/glew.h> //no extensions are available for Windows so use glew
#else
#    define GL_GLEXT_PROTOTYPES
#    include <GL/gl.h>
#    include <GL/glext.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory>
#include <cassert>
#include <sstream>
#include <array>

#if defined(__EMSCRIPTEN__)
#   include <emscripten.h>
#endif

#include <mathgp/mathgp.h>

#include "lib/Macros.h"