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
    elseif(MAIBO_PLATFORM_ANDROID)
        set(PLATFORM_PATH android/${CMAKE_BUILD_TYPE}/${ANDROID_NDK_ABI_NAME})
    elseif(MAIBO_PLATFORM_IOS)
        if(MAIBO_64_BIT)
            set(PLATFORM_PATH ios/${CMAKE_BUILD_TYPE}/arm64-v8a/)
        else()
            set(PLATFORM_PATH ios/${CMAKE_BUILD_TYPE}/armeabi-v7a)
        endif()
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
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${PLATFORM_PATH}"
        # ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PLATFORM_PATH}"
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
    set(THIRD_PARTY_LIB)
    file(TO_NATIVE_PATH "${MAIBO_ROOT_DIR}/third_party/lib/prebuilt/Windows" THIRD_PARTY_LIB)

    configure_file(
        "${MAIBO_ROOT_DIR}/build/tools/ExampleExe.template.user"
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user"
        @ONLY
    )
endmacro(configure_msvc_user_file)

# maibo_set_c_cxx_flags
#
# sets the C and C++ compilation flags to ones appropriate for maibo
#
macro(maibo_set_c_cxx_flags)
    if(MAIBO_PLATFORM_EMSCRIPTEN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-warn-absolute-paths -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s PRECISE_F32=1")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-warn-absolute-paths -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s PRECISE_F32=1")

        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g4")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g4")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
    elseif(MSVC)
        # Nothing for now
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
    endif()
endmacro(maibo_set_c_cxx_flags)

# maibo_add_executable
#
# Wraps the add_executable() command.
# Use this, instead of add_executable() to create an application.
# * android: the executable is actually a shared library. Also adds android main
# * iOS: the executable needs additional parameter to mark it as a bundle.
# * windows: the executable needs additional parameter to mark is as a windows application.
# * unix and emscripten: nothing special. Just calls add_executable(${OUT_NAME} ${SOURCES})
# Args:
# OUT_NAME - the name of the executable
# SOURCES - list of all sources
#
# Usage:
# maibo_add_executable(${APP_OUT_NAME} "${SOURCES}")
# Note: the quotes around your 'sources' variable are needed.
#
macro(maibo_add_executable OUT_NAME SOURCES)
    if(MAIBO_PLATFORM_ANDROID)
        add_library(${OUT_NAME} SHARED
            ${SOURCES}
            "${MAIBO_ROOT_DIR}/third_party/lib/platform/Android/SDL2_android_main/SDL_android_main.cpp"
        )
    elseif(MAIBO_PLATFORM_IOS)
        add_executable(${OUT_NAME} MACOSX_BUNDLE ${SOURCES})
    elseif(MAIBO_PLATFORM_WINDOWS)
        add_executable(${OUT_NAME} WIN32 ${SOURCES})
    elseif(MAIBO_PLATFORM_UNIX OR MAIBO_PLATFORM_EMSCRIPTEN)
        add_executable(${OUT_NAME} ${SOURCES})
    else()
        message("Unknown platform")
        return()
    endif()
endmacro(maibo_add_executable)
