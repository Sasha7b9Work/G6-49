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

    call clean.bat device
    call build.bat device
    
:BUILD_PANEL

if %_BUILD_PANEL_%==0 goto BUILD_DLOADER

    call clean.bat panel
    call build.bat panel

:BUILD_DLOADER

if %_BUILD_DLOADER_%==0 goto BUILD_PLOADER

    call clean.bat dloader
    call build.bat dloader

:BUILD_PLOADER

if %_BUILD_PLOADER_%==0 goto EXIT

    call clean.bat ploader
    call build.bat ploader
    goto EXIT

:HINT
    echo.
    echo Usage:
    echo       rebuild.bat [device^|panel^|dloader^|ploader^|all]
    goto EXIT

:EXIT
