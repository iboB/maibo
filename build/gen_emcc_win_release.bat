@echo off
if not exist emcc_release mkdir emcc_release
cd emcc_release
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/emscripten.toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=mingw32-make -G "Unix Makefiles" ../..
cd ..