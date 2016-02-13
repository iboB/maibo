# MaiBo
# Copyright (c) 2015 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

project(JoyPad)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${MAIBO_INCLUDE_DIRS})
link_directories(${MAIBO_LIB_DIRS})

set(sources)
src_group(JoyPad sources
    ${SRC}/JoyPadApp.cpp
    ${SRC}/JoyPadApp.h
    ${SRC}/JoyPadState.cpp
    ${SRC}/JoyPadState.h
    ${SRC}/JoyPad.pch.cpp
    ${SRC}/JoyPad.pch.h
)

src_group(PlatformerDemo sources
    ${SRC}/PlatformerDemo.cpp
    ${SRC}/PlatformerDemo.h
)

set(RES ${CMAKE_CURRENT_SOURCE_DIR}/resources)

set(all_sources
    ${sources}

    ${RES}/simple.frag
    ${RES}/simple.vert
)

maibo_add_executable(JoyPad
    "${all_sources}"
)

target_link_libraries(JoyPad ${MAIBO_LIBS})

maibo_target_properties(JoyPad)
add_precompiled_header(JoyPad ${SRC}/JoyPad.pch.h ${SRC}/JoyPad.pch.cpp)

if(MSVC)
    configure_msvc_user_file(JoyPad ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set_target_properties(JoyPad PROPERTIES FOLDER Examples)
