@echo off
if not exist vs12x64 mkdir vs12x64
cd vs12x64
cmake ../.. -G "Visual Studio 12 2013 Win64" -T "CTP_Nov2013"
cd ..