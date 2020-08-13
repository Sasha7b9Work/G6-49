@echo off

if "%1" EQU "" goto HINT
if %1==device goto BUILD_DEVICE
if %1==panel goto BUILD_PANEL
if %1==dloader goto BUILD_DLOADER
if %1==ploader goto BUILD_PLOADER

goto EXIT

:BUILD_DEVICE

    c:\Keil_v5\UV4\UV4 -b ..\..\Device\Device.uvprojx -j0 -z -o Device.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_DEVICE_SUCCESS   
    echo ERROR!!! Device    failed!!!
    type ..\..\Device\Device.out
    
:BUILD_DEVICE_SUCCESS
  
    copy ..\..\Device\G6-49-D.bin h:\
    goto EXIT

:BUILD_PANEL

    c:\Keil_v5\UV4\UV4 -b ..\..\Panel\Panel.uvprojx -j0 -z -o Panel.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_PANEL_SUCCESS
    echo ERROR!!! Panel    failed!!!
    type ..\..\Panel\Panel.out

:BUILD_PANEL_SUCCESS
    
    copy ..\..\Panel\G6-49-P.bin h:\
    goto EXIT

:BUILD_DLOADER

    c:\Keil_v5\UV4\UV4 -b ..\..\DLoader\DLoader.uvprojx -j0 -z -o DLoader.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto EXIT
    echo ERROR!!! Device Loader   failed!!!
    type ..\..\DLoader\DLoader.out
    goto EXIT

:BUILD_PLOADER

    c:\Keil_v5\UV4\UV4 -b ..\..\PLoader\PLoader.uvprojx -j0 -z -o PLoader.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto EXIT
    echo ERROR!!! Panel Loader   failed!!!
    type ..\..\PLoader\PLoader.out
    goto EXIT

:HINT
    echo.
    echo Using:
    echo       build.bat [device^|panel^|dloader^|ploader^|all]
    goto EXIT

:EXIT
