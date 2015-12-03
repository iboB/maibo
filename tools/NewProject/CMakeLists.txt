# MaiBo
# Copyright (c) 2015 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

# !!EDIT!! set your own project name here
set(PROJECT_NAME MyProject)

project(${PROJECT_NAME})

# Set maibo build options
set(MAIBO_USE_IMGUI OFF
    CACHE BOOL "ImGui third party library"
)
set(MAIBO_USE_LIBROCKET OFF
    CACHE BOOL "libRocket third party library"
)
set(MAIBO_BUILD_EXAMPLES OFF
    CACHE BOOL "Build maibo examples"
)

# Add maibo
# !!EDIT!! to set appropriate paths
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. maibo)

maibo_set_c_cxx_flags()

include_directories(${MAIBO_INCLUDE_DIRS})
link_directories(${MAIBO_LIB_DIRS})

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# !!!EDIT!!! add your own source files and groups here
set(sources)
src_group(MyGroup sources
    ${SRC}/MyApplication.cpp
    ${SRC}/MyApplication.h
    ${SRC}/MyState.cpp
    ${SRC}/MyState.h
    ${SRC}/MyProject.pch.h
    ${SRC}/MyProject.pch.cpp
)

maibo_add_executable(${PROJECT_NAME}
    "${sources}"
)

target_link_libraries(${PROJECT_NAME} ${MAIBO_LIBS})

maibo_target_properties(${PROJECT_NAME})
add_precompiled_header(${PROJECT_NAME} ${SRC}/MyProject.pch.h ${SRC}/MyProject.pch.cpp)

# Configure user file to set optional msvc specific MaiBo properties
if(MSVC)
    configure_msvc_user_file(BlockOut3000 ${CMAKE_CURRENT_SOURCE_DIR})
endif()
