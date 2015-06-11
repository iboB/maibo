# Build Instructions

## Other Notes

* Keep `CMakeSystemSpecificInformation.cmake` where it is. CMake cannot work
without it being in the appropriate relative path to a toolchain definition file 
(like emscripten.toolchain.cmake)
