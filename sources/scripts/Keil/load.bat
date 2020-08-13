@echo off

set _LOAD_DEVICE_=0
set _LOAD_PANEL_=0
set _LOAD_DLOADER_=0
set _LOAD_PLOADER_=0
set _PROCESSED_=0

if "%1" EQU "" goto HINT
if %1==device  ( set _LOAD_DEVICE_=1  & set _PROCESSED_=1 )
if %1==panel   ( set _LOAD_PANEL_=1   & set _PROCESSED_=1 )
if %1==dloader ( set _LOAD_DLOADER_=1 & set _PROCESSED_=1 )
if %1==ploader ( set _LOAD_PLOADER_=1 & set _PROCESSED_=1 )
if %1==loaders ( set _LOAD_PLOADER_=1 & set _LOAD_DLOADER_=1 & set _PROCESSED_=1 )
if %1==all     ( set _LOAD_DEVICE_=1 & set _LOAD_PANEL_=1 & set _LOAD_PLOADER_=1 & set _LOAD_DLOADER_=1 & set _PROCESSED_=1 )

if %_PROCESSED_%==0 goto HINT

if %_LOAD_DEVICE_%==0 goto LOAD_PANEL

:LOAD_DEVICE

    echo Load firmware to Device
    c:\Keil_v5\UV4\UV4 -f ..\..\Device\Device.uvprojx -j0 -o ..\scripts\Keil\out.txt

:LOAD_PANEL

if %_LOAD_PANEL_%==0 goto LOAD_DLOADER

    echo Load firmware to Panel
    c:\Keil_v5\UV4\UV4 -f ..\..\Panel\Panel.uvprojx -j0 -o ..\scripts\Keil\out.txt

:LOAD_PLOADER

if %_LOAD_PLOADER_%==0 goto LOAD_DLOADER

    echo Load firmware to Panel Loader
    c:\Keil_v5\UV4\UV4 -f ..\..\PLoader\PLoader.uvprojx -j0 -o ..\scripts\Keil\out.txt
    goto EXIT
    
:LOAD_DLOADER

if %_LOAD_DLOADER_%==0 goto EXIT

    echo Load firmware to Device Loader
    c:\Keil_v5\UV4\UV4 -f ..\..\DLoader\DLoader.uvprojx -j0 -o ..\scripts\Keil\out.txt
    goto EXIT
    
:HINT

    echo.
    echo Usage:
    echo       load [device^|panel^|dloader^|ploader^|loaders^|all]
    echo.
    goto EXIT

:EXIT

