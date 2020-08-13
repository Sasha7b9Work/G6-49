@echo off

if "%1" EQU "" goto HINT
if %1==device  goto LOAD_DEVICE
if %1==panel   goto LOAD_PANEL
if %1==dloader goto LOAD_DLOADER
if %1==ploader goto LOAD_PLOADER
if %1==loaders ( call load.bat ploader & call load.bat dloader )
if %1==all     ( call load.bat device & call load.bat panel & call load.bat ploader & call load.bat dloader & goto EXIT )

goto HINT

:LOAD_DEVICE

    echo Load firmware to Device
    c:\Keil_v5\UV4\UV4 -f ..\..\Device\Device.uvprojx -j0 -o ..\scripts\Keil\out.txt
    goto EXIT

:LOAD_PANEL


    echo Load firmware to Panel
    c:\Keil_v5\UV4\UV4 -f ..\..\Panel\Panel.uvprojx -j0 -o ..\scripts\Keil\out.txt
    goto EXIT

:LOAD_PLOADER

    echo Load firmware to Panel Loader
    c:\Keil_v5\UV4\UV4 -f ..\..\PLoader\PLoader.uvprojx -j0 -o ..\scripts\Keil\out.txt
    goto EXIT
    
:LOAD_DLOADER

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

