@echo off

if "%2" EQU "" goto LABEL1
if %2==edit goto BUILD_EDIT
goto HINT

:LABEL1
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
    echo       make [build [edit]]
    echo.
    goto EXIT

:BUILD_EDIT
    if %1==build ( call make.bat build & call edit.bat & goto EXIT )
    goto HINT

:EXIT
