# MaiBo
# Copyright (c) 2015 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

project(BlockOut3000)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${MAIBO_INCLUDE_DIRS})
link_directories(${MAIBO_LIB_DIRS})

set(sources)
src_group(BlockOut sources
    ${SRC}/BlockOut.cpp
    ${SRC}/BlockOut.h
    ${SRC}/BlockOut.pch.cpp
    ${SRC}/BlockOut.pch.h
)

add_executable(BlockOut3000
    ${sources}
)

target_link_libraries(BlockOut3000 ${MAIBO_LIBS})

maibo_target_properties(BlockOut3000)
add_precompiled_header(BlockOut3000 ${SRC}/BlockOut.pch.h ${SRC}/BlockOut.pch.cpp)

if(MSVC)
    configure_msvc_user_file(BlockOut3000 ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set_target_properties(BlockOut3000 PROPERTIES FOLDER Examples)
