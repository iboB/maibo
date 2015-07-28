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
        if(MAIBO_64_BIT)
            set(PLATFORM_PATH windows/x64)
        else()
            set(PLATFORM_PATH windows/x86)
        endif()
    elseif(MAIBO_PLATFORM_UNIX)
        if(MAIBO_64_BIT)
            set(PLATFORM_PATH linux/amd64/${CMAKE_BUILD_TYPE})
        else()
            set(PLATFORM_PATH linux/i386/${CMAKE_BUILD_TYPE})
        endif()
    endif()

    set_target_properties(${TARGET_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PLATFORM_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${PLATFORM_PATH}"
    )
endmacro(maibo_target_properties)

# maibo_target_properties
#
# Configures a TARGET_NAME.vcxproj.user file with msvc specific settings
# that belong there.
# The macro uses the template file \build\Tools\ExampleExe.template.user
# Args:
# WORKING_DIR - the working directory when debugging the executable with the Visual Studio debugger
#
macro(configure_msvc_user_file TARGET_NAME WORKING_DIR)
    set(USERFILE_WORKING_DIR ${WORKING_DIR})

    configure_file(
        "${MAIBO_ROOT_DIR}/build/tools/ExampleExe.template.user"
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user"
        @ONLY
    )
endmacro(configure_msvc_user_file)
