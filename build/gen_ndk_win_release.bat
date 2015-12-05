@echo off
if not exist ndk_release mkdir ndk_release
cd ndk_release
cmake -DCMAKE_BUILD_TYPE=Release -DANDROID_NATIVE_API_LEVEL="android-14" -DCMAKE_TOOLCHAIN_FILE=../toolchains/android.toolchain.cmake -DCMAKE_MAKE_PROGRAM=%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe -DANDROID_ABI="armeabi-v7a" -G "MinGW Makefiles" ../..
cd ..
