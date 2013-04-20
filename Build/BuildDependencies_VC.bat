@echo off

cd %~dp0
echo wxWidgets
echo =========
cd ..\Dependencies\wxWidgets\build\msw

nmake -f makefile.vc BUILD=release MONOLITHIC=0 SHARED=0 UNICODE=1
nmake -f makefile.vc BUILD=debug MONOLITHIC=0 SHARED=0 UNICODE=1

cd %~dp0
echo.
echo libcurl
echo =======
cd ..\Dependencies\libcurl\winbuild

nmake /f Makefile.vc mode=static
nmake /f Makefile.vc mode=static debug=yes

if exist "..\builds\libcurl-vc-x86-debug-static-ipv6-sspi-spnego-winssl\lib\libcurl_a_debug.lib" (
    xcopy "..\builds\libcurl-vc-x86-debug-static-ipv6-sspi-spnego-winssl\lib\libcurl_a_debug.lib" "..\lib\debug\*" /Y
)

if exist "..\builds\libcurl-vc-x86-release-static-ipv6-sspi-spnego-winssl\lib\libcurl_a.lib" (
    xcopy "..\builds\libcurl-vc-x86-release-static-ipv6-sspi-spnego-winssl\lib\libcurl_a.lib" "..\lib\release\*" /Y
)

if exist "..\builds\libcurl-vc-x64-debug-static-ipv6-sspi-spnego-winssl\lib\libcurl_a_debug.lib" (
    xcopy "..\builds\libcurl-vc-x64-debug-static-ipv6-sspi-spnego-winssl\lib\libcurl_a_debug.lib" "..\lib\debug\*" /Y
)

if exist "..\builds\libcurl-vc-x64-release-static-ipv6-sspi-spnego-winssl\lib\libcurl_a.lib" (
    xcopy "..\builds\libcurl-vc-x64-release-static-ipv6-sspi-spnego-winssl\lib\libcurl_a.lib" "..\lib\release\*" /Y
)

cd %~dp0
echo.
echo Done!
pause
