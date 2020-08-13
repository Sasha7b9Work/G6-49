@echo off

if "%1" EQU "" goto HINT
if %1==device  ( call clean.bat %1 & call build.bat %1 & goto EXIT )
if %1==panel   ( call clean.bat %1 & call build.bat %1 & goto EXIT )
if %1==dloader ( call clean.bat %1 & call build.bat %1 & goto EXIT )
if %1==ploader ( call clean.bat %1 & call build.bat %1 & goto EXIT )
if %1==all     ( call rebuild.bat device & call rebuild.bat panel & call rebuild.bat ploader & call rebuild.bat dloader & goto EXIT )

goto HINT

:HINT
    echo.
    echo Usage:
    echo       rebuild.bat [device^|panel^|dloader^|ploader^|all]
    goto EXIT

:EXIT
