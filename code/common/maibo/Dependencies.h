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
#if defined(_MSC_VER) || defined(MAIBO_PLATFORM_MOBILE)
#   include <SDL.h>
#   include <SDL_image.h>
#else
#   include <SDL2/SDL.h>
#   include <SDL2/SDL_image.h>
#endif

#if defined(_WIN32)
#    include <GL/glew.h> //no extensions are available for Windows so use glew
#elif defined(ANDROID)
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#   include <GLES2/gl2platform.h>
#else
#    define GL_GLEXT_PROTOTYPES
#    include <GL/gl.h>
#    include <GL/glext.h>
#endif

#if defined(__APPLE__)
#   include "TargetConditionals.h"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <list>
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

#include <yama/yama.hpp>

namespace yama
{
typedef vector2_t<uint32_t> uvector2;
typedef vector3_t<uint32_t> uvector3;
typedef vector4_t<float> float4;
typedef vector3_t<int32_t> ivector3;
}

#include "Common/Macros.h"