@echo off

cd %~dp0
echo wxWidgets
echo =========
cd ..\Dependencies\wxWidgets\build\msw
mingw32-make -f makefile.gcc

cd %~dp0
echo.
echo libcurl
echo =======
cd ..\Dependencies\libcurl
mingw32-make mingw32
xcopy "lib\libcurl.a" "lib\debug\*" /Y
xcopy "lib\libcurl.a" "lib\release\*" /Y

cd %~dp0
echo.
echo Done!
pause
