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
    ${SRC}/LoadAllState.cpp
    ${SRC}/LoadAllState.h
    ${SRC}/Resources.cpp
    ${SRC}/Resources.h
    ${SRC}/Preferences.cpp
    ${SRC}/Preferences.h
)

src_group(Game sources
    ${SRC}/PlayingState.cpp
    ${SRC}/PlayingState.h
    ${SRC}/Level.cpp
    ${SRC}/Level.h
)

src_group(Game~Figure sources
    ${SRC}/CubeTemplate.cpp
    ${SRC}/CubeTemplate.h
    ${SRC}/Primitives.h
    ${SRC}/FigureTemplate.cpp
    ${SRC}/FigureTemplate.h
    ${SRC}/FigureSet.cpp
    ${SRC}/FigureSet.h
    ${SRC}/FigureManager.cpp
    ${SRC}/FigureManager.h
    ${SRC}/Figure.cpp
    ${SRC}/Figure.h
    ${SRC}/SimpleLexer.cpp
    ${SRC}/SimpleLexer.h
    ${SRC}/LoadFigureSetsTask.cpp
    ${SRC}/LoadFigureSetsTask.h
)

src_group(Rendering sources
    ${SRC}/UniformColorMaterial.cpp
    ${SRC}/UniformColorMaterial.h
)


set(RES ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_executable(BlockOut3000
    ${sources}

    ${RES}/u_color.frag
    ${RES}/pos.vert
    ${RES}/figures.dat
)

target_link_libraries(BlockOut3000 ${MAIBO_LIBS})

maibo_target_properties(BlockOut3000)
add_precompiled_header(BlockOut3000 ${SRC}/BlockOut.pch.h ${SRC}/BlockOut.pch.cpp)

if(MSVC)
    configure_msvc_user_file(BlockOut3000 ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set_target_properties(BlockOut3000 PROPERTIES FOLDER Examples)
