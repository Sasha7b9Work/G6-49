@echo off

if "%1" EQU "" goto MAKE
if %1==build goto MAKE
goto HINT

:MAKE
    @echo.
    @echo %TIME%   Making Visual Studio Win Debug ...
    
    cd ../..
    
    rmdir "generated/Win32" /s /q
    
    cd scripts/vs_win
    
    @echo on
    cmake ../../VS/CMakeLists.txt -B../../generated/Win32 -G "Visual Studio 16 2019" -A Win32
    
    @echo off
    
    @echo %TIME%   Complete
    
    if "%1" EQU "" goto EXIT
    if %1==build goto BUILD
    goto HINT
    

:BUILD
    call build.bat
    goto EXIT

:HINT
    echo.
    echo Using:
    echo       make [build]
    echo.
    goto EXIT

:EXIT
