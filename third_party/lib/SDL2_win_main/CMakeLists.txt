# manually compiled SDL main since we don't
# want to conform to the link choices of the prebuilt lib

project(SDL2_main)

add_library(SDL2_main STATIC
    SDL_windows_main.c
    SDL_windows.c
    SDL_windows.h
)

set(SDL2_MAIN_LINK_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    CACHE STRING "SDL2 Windows main link path"
)

set(SDL2_MAIN_LIB
    SDL2_main
    CACHE STRING "SDL2 Windows main library"
)

set_target_properties(SDL2_main PROPERTIES FOLDER ThirdParty)
