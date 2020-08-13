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

:BUILD_DEVICE

if %_BUILD_DEVICE_%==0 goto BUILD_PANEL

    call build.bat device
    call load.bat device

echo 1
    
:BUILD_PANEL

echo 2

if %_BUILD_PANEL_%==0 goto BUILD_DLOADER

echo 3

    call build.bat panel
    call load.bat panel

:BUILD_DLOADER

echo 4

if %_BUILD_DLOADER_%==0 goto BUILD_PLOADER

    call build.bat dloader
    call load.bat dloader

:BUILD_PLOADER

if %_BUILD_PLOADER_%==0 goto EXIT

    call build.bat ploader
    call load.bat ploader
    goto EXIT

:HINT
    echo.
    echo Usage:
    echo       build_load.bat [device^|panel^|dloader^|ploader^|all]
    goto EXIT

:EXIT

echo 5
