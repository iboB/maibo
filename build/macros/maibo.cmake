# MaiBo
# Copyright (c) 2015 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
# maibo_target_properties
#
# sets MaiBo specific properties for a given target
# Args:
# TARGET_NAME - the target
#
macro(maibo_target_properties TARGET_NAME)

    set(PLATFORM_PATH) # path to be appended to the output directory

    if(MAIBO_PLATFORM_EMSCRIPTEN)
        set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2")
        set(PLATFORM_PATH emscripten/${CMAKE_BUILD_TYPE})
    elseif(MAIBO_PLATFORM_WINDOWS)
        set(PLATFORM_PATH windows)
    endif()

    set_target_properties(${TARGET_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PLATFORM_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${PLATFORM_PATH}"
    )
endmacro(maibo_target_properties)
