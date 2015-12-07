# MaiBo
# Copyright (c) 2015 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#

# Third party libraries

set(PREBUILT_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt)

if(MSVC)
    add_subdirectory(platform/Windows/SDL2_win_main)

    set(platformIncDirs
        ${PREBUILT_LIBS}/Windows/glew-1.11.0/include
        ${PREBUILT_LIBS}/Windows/SDL2-2.0.3/include
        ${PREBUILT_LIBS}/Windows/SDL2_image-2.0.0/include
    )

    if(MAIBO_64_BIT)
        set(platformLinkDirs
            ${PREBUILT_LIBS}/Windows/glew-1.11.0/lib/Release/x64
            ${PREBUILT_LIBS}/Windows/SDL2_image-2.0.0/lib/x64
            ${PREBUILT_LIBS}/Windows/SDL2-2.0.3/lib/x64
            ${SDL2_MAIN_LINK_DIR}
        )
    else()
        set(platformLinkDirs
            ${PREBUILT_LIBS}/Windows/glew-1.11.0/lib/Release/x86
            ${PREBUILT_LIBS}/Windows/SDL2_image-2.0.0/lib/x86
            ${PREBUILT_LIBS}/Windows/SDL2-2.0.3/lib/x86
            ${SDL2_MAIN_LINK_DIR}
        )
    endif()

    set(platformLibs
        glew32.lib
        SDL2.lib
        SDL2_image.lib
        ${SDL2_MAIN_LIB}

        opengl32.lib
    )
elseif(MAIBO_PLATFORM_UNIX)
    set(platformLibs
        SDL2.so
        GL
    )
elseif(MAIBO_PLATFORM_ANDROID)
    set(platformIncDirs
        ${PREBUILT_LIBS}/Android/include
    )

    set(platformLinkDirs
        ${PREBUILT_LIBS}/Android/libs/${ANDROID_NDK_ABI_NAME}
    )

    set(platformLibs
        SDL2
        SDL2_image

        GLESv2
    )
endif()

if(MAIBO_USE_IMGUI)
    add_subdirectory(imgui)
endif(MAIBO_USE_IMGUI)

if(MAIBO_USE_LIBROCKET)
    add_subdirectory(freetype)
    set(ROCKET_BUILD_SHARED_LIBS OFF
        CACHE BOOL "Build libRocket shared"
    )
    add_subdirectory(libRocket)
endif(MAIBO_USE_LIBROCKET)

set(THIRD_PARTY_INCLUDE_DIRS
    ${platformIncDirs}
    ${IMGUI_INCLUDE_DIR}
    ${LIBROCKET_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mathgp
    CACHE STRING "MaiBo third party include paths"
)

set(THIRD_PARTY_DEFINITIONS
    ${LIBROCKET_DEFINITIONS}
    CACHE STRING "Maibo definitions"
)

set(THIRD_PARTY_LINK_DIRS
    ${platformLinkDirs}
    ${IMGUI_LINK_DIR}
    ${LIBROCKET_LINK_DIR}
    CACHE STRING "MaiBo third party link paths"
)

set(THIRD_PARTY_LIBS
    ${platformLibs}
    ${IMGUI_LIB}
    ${LIBROCKET_LIBS}
    CACHE STRING "MaiBo third party libraries"
)
