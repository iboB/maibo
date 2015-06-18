:: Copy the third party dlls required for the
:: application to run.
@echo off

call :copyx64 Debug if exist bin\windows\x64\Debug

goto :eof

:: arg 1 is configuration Debug|Release|RelWithDebugInfo

:copyx64
copy ..\..\third_party\lib\glew-1.11.0\bin\Release\x64\glew32.dll bin\windows\x64\%~1\
copy ..\..\third_party\lib\SDL2-2.0.3\lib\x64\SDL2.dll bin\windows\x64\%~1\
copy ..\..\third_party\lib\SDL2_image-2.0.0\lib\x64\SDL2_image.dll bin\windows\x64\%~1\
goto :eof

:copyx86
copy ..\..\third_party\lib\glew-1.11.0\bin\Release\Win32\glew32.dll bin\windows\x86\%~1\
copy ..\..\third_party\lib\SDL2-2.0.3\lib\x86\SDL2.dll bin\windows\x86\%~1\
copy ..\..\third_party\lib\SDL2_image-2.0.0\lib\x86\SDL2_image.dll bin\windows\x86\%~1\
goto :eof