@echo off

set _BUILD_DEVICE_=0
set _BUILD_PANEL_=0
set _BUILD_DLOADER_=0
set _BUILD_PLOADER_=0
set _PROCESSED_=0

if "%1" EQU "" goto HINT
if %1==device  ( set _BUILD_DEVICE_=1  & set _PROCESSED_=1 )
if %1==panel   ( set _BUILD_PANEL_=1   & set _PROCESSED_=1 )
if %1==dloader ( set _BUILD_DLOADER_=1 & set _PROCESSED_=1 )
if %1==ploader ( set _BUILD_PLOADER_=1 & set _PROCESSED_=1 )
if %1==all     ( set _BUILD_DEVICE_=1  & set _BUILD_PANEL_=1 & set _BUILD_DLOADER_=1 & set _BUILD_PLOADER_=1 & set _PROCESSED_=1 )

if %_PROCESSED_%==0 goto HINT

if %_BUILD_DEVICE_%==0 goto BUILD_PANEL

:BUILD_DEVICE

    echo Compile Device
    c:\Keil_v5\UV4\UV4 -b ..\..\Device\Device.uvprojx -j0 -z -o Device.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_DEVICE_SUCCESS
    echo ERROR!!! Device    failed!!!
    type ..\..\Device\Device.out
    
:BUILD_DEVICE_SUCCESS
  
    copy ..\..\Device\G6-49-D.bin h:\

:BUILD_PANEL

if %_BUILD_PANEL_%==0 goto BUILD_DLOADER

    echo Compile Panel
    c:\Keil_v5\UV4\UV4 -b ..\..\Panel\Panel.uvprojx -j0 -z -o Panel.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_PANEL_SUCCESS
    echo ERROR!!! Panel    failed!!!
    type ..\..\Panel\Panel.out

:BUILD_PANEL_SUCCESS
    
    copy ..\..\Panel\G6-49-P.bin h:\

:BUILD_DLOADER

if %_BUILD_DLOADER_%==0 goto BUILD_PLOADER

    echo Compile Device Loader
    c:\Keil_v5\UV4\UV4 -b ..\..\DLoader\DLoader.uvprojx -j0 -z -o DLoader.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_PLOADER
    echo ERROR!!! Device Loader   failed!!!
    type ..\..\DLoader\DLoader.out

:BUILD_PLOADER

if %_BUILD_PLOADER_%==0 goto EXIT

    echo Compile Panel Loader
    c:\Keil_v5\UV4\UV4 -b ..\..\PLoader\PLoader.uvprojx -j0 -z -o PLoader.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto EXIT
    echo ERROR!!! Panel Loader   failed!!!
    type ..\..\PLoader\PLoader.out
    goto EXIT

:HINT
    echo.
    echo Usage:
    echo       build.bat [device^|panel^|dloader^|ploader^|all]
    goto EXIT

:EXIT
