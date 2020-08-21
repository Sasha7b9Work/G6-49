@echo off

if "%1" EQU "" goto MAKE
if %1==build goto MAKE
if %1==rebuild goto MAKE
goto HINT

:MAKE
    cd vs_keil
    call make.bat
    cd ../vs_win
    call make.bat
    cd ..
    if "%1" EQU "" goto EXIT
    if %1==build ( call build.bat & goto EXIT )
    if %1==rebuild ( call rebuild.bat & goto EXIT )
    goto HINT

:HINT
    echo.
    echo Usage:
    echo       make.bat [build^|rebuild]
    echo.
    goto EXIT

:EXIT
