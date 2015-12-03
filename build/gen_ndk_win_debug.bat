@echo off
if not exist ndk_debug mkdir ndk_debug
cd ndk_debug
cmake -DCMAKE_BUILD_TYPE=Debug -DANDROID_NATIVE_API_LEVEL="android-14" -DCMAKE_TOOLCHAIN_FILE=../toolchains/android.toolchain.cmake -DCMAKE_MAKE_PROGRAM=%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe -DANDROID_ABI="armeabi-v7a" -G "MinGW Makefiles" ../..
cd ..
