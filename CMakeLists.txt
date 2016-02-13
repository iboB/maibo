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
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/code/maibo)

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
    ${SRC}/Application.h
    ${SRC}/MaiBo.pch.h
    ${SRC}/MaiBo.pch.cpp
    ${SRC}/Dependencies.h
    ${SRC}/MainWindow.cpp
    ${SRC}/MainWindow.h
)

src_group(Resources sources
    ${SRC}/ResourceManager.cpp
    ${SRC}/ResourceManager.h
    ${SRC}/ResourceFuture.h
    ${SRC}/ResourceTask.h
)

src_group(Lib sources
    ${SRC}/lib/ExplicitSingleton.h
    ${SRC}/lib/ImplicitSingleton.h
    ${SRC}/lib/high_res_clock.h
    ${SRC}/lib/high_res_clock.cpp
    ${SRC}/lib/memory.h
    ${SRC}/lib/GLSentries.h
    ${SRC}/lib/Macros.h
    ${SRC}/lib/simplified_rng.h
    ${SRC}/lib/string.h
)

src_group(Lib~Signals sources
    ${SRC}/lib/Signals/Signal.h
    ${SRC}/lib/Signals/AutoSlot.h
    ${SRC}/lib/Signals/Slot.h
)

src_group(Tasks sources
    ${SRC}/TaskManager.cpp
    ${SRC}/TaskManager.h
    ${SRC}/Task.h
    ${SRC}/Future.h
)

src_group(States sources
    ${SRC}/AppState.h
    ${SRC}/ResourceFutureState.cpp
    ${SRC}/ResourceFutureState.h
)

src_group(Rendering sources
    ${SRC}/Texture.cpp
    ${SRC}/Texture.h
    ${SRC}/TexturePtr.h
)

src_group(Rendering~Material sources
    ${SRC}/Shader.cpp
    ${SRC}/Shader.h
    ${SRC}/ShaderPtr.h
    ${SRC}/ShaderType.h
    ${SRC}/GPUProgram.cpp
    ${SRC}/GPUProgram.h
    ${SRC}/GPUProgramPtr.h
)

src_group(Rendering~RenderTarget sources
    ${SRC}/RenderTarget.cpp
    ${SRC}/RenderTarget.h
    ${SRC}/RenderTargetManager.cpp
    ${SRC}/RenderTargetManager.h
)

src_group(Input sources
    ${SRC}/InputEventHandler.h
)

src_group(JoyPad sources
    ${SRC}/joypad/Action.h
    ${SRC}/joypad/JoyPad.cpp
    ${SRC}/joypad/JoyPad.h
    ${SRC}/joypad/RenderInterface.h
    ${SRC}/joypad/RenderTypes.h
)

if(MAIBO_USE_IMGUI)
    src_group(GUI~ImGui sources
        ${SRC}/GUI/ImGui/ImGuiManager.cpp
        ${SRC}/GUI/ImGui/ImGuiManager.h
    )
endif(MAIBO_USE_IMGUI)

# third party libraries
add_subdirectory(third_party/lib)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
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
add_precompiled_header(maibo ${SRC}/MaiBo.pch.h ${SRC}/MaiBo.pch.cpp)

set(MAIBO_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/code
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
