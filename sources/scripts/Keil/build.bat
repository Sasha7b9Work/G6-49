@echo off

if "%1" EQU "" goto HINT
if %1==device  ( call :BUILD Device  Device ..\..\Device\G6-49-D.bin & goto EXIT )
if %1==panel   ( call :BUILD Panel   Panel ..\..\Panel\G6-49-P.bin   & goto EXIT )
if %1==dloader ( call :BUILD DLoader DLoader & goto EXIT )
if %1==ploader ( call :BUILD PLoader PLoader & goto EXIT )
if %1==all     ( call build.bat device & call build.bat panel & call build.bat dloader & call build.bat ploader & goto EXIT)

goto HINT

:HINT
    echo.
    echo Usage:
    echo       build.bat [device^|panel^|dloader^|ploader^|all]
    goto EXIT

:BUILD
    echo Compile %1
    c:\Keil_v5\UV4\UV4 -b ..\..\%2\%2.uvprojx -j0 -z -o %2.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_SUCCESS
    echo ERROR!!! %2    failed!!!
    type ..\..\%2\%2.out
    exit /b
:BUILD_SUCCESS
    if "%3" EQU "" exit /b
    echo on
    copy %3 h:\
    echo off
    exit /b

:EXIT
