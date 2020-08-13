@echo off

set _CLEAN_DEVICE_=0
set _CLEAN_PANEL_=0
set _CLEAN_PLOADER_=0
set _CLEAN_DLOADER_=0
set _PROCESSED_=0

if "%1" EQU "" goto HINT

if %1==device  ( set _CLEAN_DEVICE_=1  & set _PROCESSED_=1 )
if %1==panel   ( set _CLEAN_PANEL_=1   & set _PROCESSED_=1 )
if %1==dloader ( set _CLEAN_DLOADER_=1 & set _PROCESSED_=1 )
if %1==ploader ( set _CLEAN_PLOADER_=1 & set _PROCESSED_=1 )
if %1==all     ( set _CLEAN_DEVICE_=1 & set _CLEAN_PANEL_= 1 & set _CLEAN_DLOADER_=1 & set _CLEAN_PLOADER_=1 & set _PROCESSED_=1 )

if %_PROCESSED_%==0 goto HINT

:CLEAN_PANEL

if %_CLEAN_PANEL_%==0 goto CLEAN_DEVICE

    echo Clean Panel
    c:\Keil_v5\UV4\UV4 -c..\..\Panel\Panel.uvprojx -j0

:CLEAN_DEVICE

if %_CLEAN_DEVICE_%==0 goto CLEAN_DLOADER

    echo Clean Device
    c:\Keil_v5\UV4\UV4 -c..\..\Device\Device.uvprojx -j0

:CLEAN_DLOADER

if %_CLEAN_DLOADER_%==0 goto CLEAN_PLOADER

    echo Clean Device Loader
    c:\Keil_v5\UV4\UV4 -c..\..\DLoader\DLoader.uvprojx -j0

:CLEAN_PLOADER

if %_CLEAN_PLOADER_%==0 goto EXIT

    echo Clean Panel Loader
    c:\Keil_v5\UV4\UV4 -c..\..\PLoader\PLoader.uvprojx -j0
    goto EXIT

:HINT

    echo.
    echo Usage:
    echo       clean [panel^|device^|dloader^|ploader^|all]
    echo.
    goto EXIT

:EXIT
