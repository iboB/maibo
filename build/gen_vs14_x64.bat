@echo off
if not exist vs14x64 mkdir vs14x64
cd vs14x64
cmake ../.. -G "Visual Studio 14 2015 Win64"
cd ..