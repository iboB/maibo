# Third party libraries

if(MSVC)
    add_subdirectory(SDL2_win_main)

    set(platformIncDirs
        ${CMAKE_CURRENT_SOURCE_DIR}/glew-1.11.0/include
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.3/include
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image-2.0.0/include
    )

    if(MAIBO_64_BIT)
        set(platformLinkDirs
            ${CMAKE_CURRENT_SOURCE_DIR}/glew-1.11.0/lib/Release/x64
            ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image-2.0.0/lib/x64
            ${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.3/lib/x64
            ${SDL2_MAIN_LINK_DIR}
        )
    else()
        set(platformLinkDirs
            ${CMAKE_CURRENT_SOURCE_DIR}/glew-1.11.0/lib/Release/x86
            ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image-2.0.0/lib/x86
            ${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.3/lib/x86
            ${SDL2_MAIN_LINK_DIR}
        )
    endif()

    set(platformLibs
        glew32.lib
        SDL2.lib
        SDL2_Image.lib
        ${SDL2_MAIN_LIB}

        opengl32.lib
    )
endif()

set(THIRD_PARTY_INCLUDE_DIRS
    ${platformIncDirs}
    CACHE STRING "MaiBo third party include paths"
)

set(THIRD_PARTY_LINK_DIRS
    ${platformLinkDirs}
    CACHE STRING "MaiBo third party link paths"

)

set(THIRD_PARTY_LIBS
    ${platformLibs}
    CACHE STRING "MaiBo third party libraries"
)
