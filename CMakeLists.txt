# MaiBo
# Copyright (c) 2015 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(maibo)

set(MAIBO_ROOT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE STRING "Root directory of MaiBo")

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${MAIBO_ROOT_DIR}/build/macros
    ${MAIBO_ROOT_DIR}/third_party/build/ucm/cmake
)

include(maibo)
include(precompiled_header)
include(src_group)
include(ucm)

# Options about components
# All are enabled by default
option(MAIBO_USE_IMGUI "Integrate the ImGui third party library" ON)

# Options about example projects
# All are enabled by default
option(MAIBO_BUILD_EXAMPLES "Build MaiBo examples" ON)

# Infividual example settings. Only relevant if MAIBO_BUILD_EXAMPLES is ON
if(MAIBO_BUILD_EXAMPLES)
    option(MAIBO_EXAMPLE_BLOCKOUT3000 "Also build the example BlockOut3000" ON)
    option(MAIBO_EXAMPLE_JOYPAD "Also build the example JoyPad" ON)
endif()

# Set target platform
if(EMSCRIPTEN)
    set(MAIBO_PLATFORM_EMSCRIPTEN 1 CACHE INTERNAL "Platform is Emscripten")
elseif(ANDROID)
    set(MAIBO_PLATFORM_ANDROID 1 CACHE INTERNAL "Platform is Android")
    set(MAIBO_PLATFORM_MOBILE 1 CACHE INTERNAL "Platform is mobile")
elseif(IOS)
    set(MAIBO_PLATFORM_IOS 1 CACHE INTERNAL "Platform is IOS")
    set(MAIBO_PLATFORM_MOBILE 1 CACHE INTERNAL "Platform is mobile")
elseif(CMAKE_HOST_WIN32)
    set(MAIBO_PLATFORM_WINDOWS 1 CACHE INTERNAL "Platform is Windows")
    set(MAIBO_PLATFORM_DESKTOP 1 CACHE INTERNAL "Platform is Desktop")
elseif(CMAKE_HOST_UNIX)
    set(MAIBO_PLATFORM_UNIX 1 CACHE INTERNAL "Platform is Unix")
    set(MAIBO_PLATFORM_DESKTOP 1 CACHE INTERNAL "Platform is Desktop")
else()
    message(FATAL_ERROR "Unknown target platform for the MAIBO build")
endif()

# Set target bitness
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MAIBO_64_BIT 1 CACHE INTERNAL "Platform is 64-bit")
else()
    set(MAIBO_32_BIT 1 CACHE INTERNAL "Platform is 32-bit")
endif()

# Set a default build type for non-multi build type platforms
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set platform-specific compiler settings
maibo_set_c_cxx_flags()

# Local variables for shorter filenames
set(SRC code/common/maibo)

# Set platform specific sources, include dirs, library dirs, and link libraries
set(platformSources)
if(MAIBO_PLATFORM_EMSCRIPTEN)
elseif(MAIBO_PLATFORM_WINDOWS)
elseif(MAIBO_PLATFORM_UNIX)
endif()

# common sources
set(sources)
ucm_add_dirs(${SRC} TO sources RECURSIVE FILTER_POP 3)

ucm_add_dirs(plugins/JoyPad/code/maibo TO sources RECURSIVE FILTER_POP 4)
set(PLUGIN_INCLUDE_DIRS plugins/JoyPad/code)

if(MAIBO_USE_IMGUI)
    ucm_add_dirs(plugins/ImGui/code/maibo TO sources RECURSIVE FILTER_POP 4)
    set(PLUGIN_INCLUDE_DIRS ${PLUGIN_INCLUDE_DIRS} plugins/ImGui/code)
endif(MAIBO_USE_IMGUI)

# third party libraries
add_subdirectory(third_party/lib)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code/common
    ${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_INCLUDE_DIRS}
    ${THIRD_PARTY_INCLUDE_DIRS}
)

add_definitions(${THIRD_PARTY_DEFINITIONS})

# add definitions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(MAIBO_PLATFORM_DESKTOP)
    add_definitions(-DMAIBO_PLATFORM_DESKTOP)
elseif(MAIBO_PLATFORM_MOBILE)
    add_definitions(-DMAIBO_PLATFORM_MOBILE)
endif()

add_library(maibo STATIC
    ${platformSources}
    ${sources}
)

maibo_target_properties(maibo)
add_precompiled_header(maibo ${MAIBO_ROOT_DIR}/${SRC}/pch/MaiBo.pch.h ${MAIBO_ROOT_DIR}/${SRC}/pch/MaiBo.pch.cpp)

set(MAIBO_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/code/common
    ${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_INCLUDE_DIRS}
    ${THIRD_PARTY_INCLUDE_DIRS}
    CACHE STRING "MaiBo include paths"
)

set(MAIBO_LIB_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${THIRD_PARTY_LINK_DIRS}
    CACHE STRING "MaiBo library paths"
)

set(MAIBO_LIBS
    maibo
    ${THIRD_PARTY_LIBS}
    CACHE STRING "MaiBo link libraries"
)

if(MAIBO_EXAMPLE_BLOCKOUT3000)
    add_subdirectory(${MAIBO_ROOT_DIR}/examples/BlockOut3000)
endif()

if(MAIBO_EXAMPLE_JOYPAD)
    add_subdirectory(${MAIBO_ROOT_DIR}/examples/JoyPad)
endif()
