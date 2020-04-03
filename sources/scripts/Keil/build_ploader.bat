@echo off
@echo %TIME%   Compile Panel Loader ...

c:\Keil_v5\UV4\UV4 -b ..\..\PLoader\PLoader.uvprojx -j0 -z -o PLoader.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Panel Loader   failed!!!

type ..\..\PLoader\PLoader.out

:Exit
