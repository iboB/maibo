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

set(CMAKE_MODULE_PATH ${MAIBO_ROOT_DIR}/build/macros)
include(maibo)
include(precompiled_header)
include(src_group)

# Options about example projects
# All are enabled by default
option(MAIBO_EXAMPLE_BLOCKOUT3000 "Also build the example BlockOut3000" ON)

# Set target platform
if(EMSCRIPTEN)
    set(MAIBO_PLATFORM_EMSCRIPTEN 1)
elseif(CMAKE_HOST_WIN32)
    set(MAIBO_PLATFORM_WINDOWS 1)
    set(MAIBO_PLATFORM_DESKTOP 1)
elseif(CMAKE_HOST_UNIX)
    set(MAIBO_PLATFORM_UNIX 1)
    set(MAIBO_PLATFORM_DESKTOP 1)
else()
    message(FATAL_ERROR "Unknown target platform for the MAIBO build")
endif()

# Set target bitness
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MAIBO_64_BIT 1)
else()
    set(MAIBO_32_BIT 1)
endif()

# Set a default build type for non-multi build type platforms
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set platform-specific compiler settings
if(MAIBO_PLATFORM_EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-warn-absolute-paths -s USE_SDL=2 -s USE_SDL_IMAGE=2")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif(MSVC)
    # Nothing for now
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

# Local variables for shorter filenames
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/include/maibo)

# Set platform specific sources, include dirs, library dirs, and link libraries
set(platformSources)
if(MAIBO_PLATFORM_EMSCRIPTEN)
elseif(MAIBO_PLATFORM_WINDOWS)
elseif(MAIBO_PLATFORM_UNIX)
endif()

# common sources
set(sources)
src_group(Maibo sources
    ${SRC}/EntryPoint.cpp
    ${SRC}/Application.cpp
    ${INC}/Application.h
    ${SRC}/MaiBo.pch.h
    ${SRC}/MaiBo.pch.cpp
    ${INC}/Dependencies.h
)

src_group(Resources sources
    ${SRC}/ResourceManager.cpp
    ${INC}/ResourceManager.h
    ${INC}/ResourceFuture.h
)

src_group(Lib sources
    ${INC}/ExplicitSingleton.h
    ${INC}/ImplicitSingleton.h
)

src_group(Tasks sources
    ${SRC}/TaskManager.cpp
    ${INC}/TaskManager.h
    ${INC}/Task.h
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${THIRD_PARTY_INCLUDE_DIRS}
)

# add definitions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(MAIBO_PLATFORM_DESKTOP)
    add_definitions(-DMAIBO_PLATFORM_DESKTOP)
endif()

add_library(maibo STATIC
    ${platformSources}
    ${sources}
)

maibo_target_properties(maibo)
add_precompiled_header(maibo ${SRC}/MaiBo.pch.h ${SRC}/MaiBo.pch.cpp)

# third party libraries
add_subdirectory(third_party/lib)

set(MAIBO_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
