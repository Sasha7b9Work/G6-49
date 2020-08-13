@echo off

if "%1" EQU "" goto HINT
if %1==device  goto CLEAN_DEVICE
if %1==panel   goto CLEAN_PANEL
if %1==dloader goto CLEAN_DLOADER
if %1==ploader goto CLEAN_PLOADER
if %1==all     ( call clean.bat device & call clean.bat panel & call clean.bat dloader & call clean.bat ploader & goto EXIT )

goto HINT

:CLEAN_PANEL
    echo Clean Panel
    c:\Keil_v5\UV4\UV4 -c..\..\Panel\Panel.uvprojx -j0
    goto EXIT

:CLEAN_DEVICE
    echo Clean Device
    c:\Keil_v5\UV4\UV4 -c..\..\Device\Device.uvprojx -j0
    goto EXIT

:CLEAN_DLOADER
    echo Clean Device Loader
    c:\Keil_v5\UV4\UV4 -c..\..\DLoader\DLoader.uvprojx -j0
    goto EXIT

:CLEAN_PLOADER
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
