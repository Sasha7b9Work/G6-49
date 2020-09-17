@echo off

if "%1" EQU "" goto HINT
if %1==WX_DLLS goto COPY_WX_DLLS
goto HINT

:COPY_WX_DLLS
    @echo %TIME%   Copying dll-files wxWidgtes
    xcopy ..\..\ThirdParty\wxWidgets\generated\lib\vc_dll\wxbase314ud_vc_custom.dll ..\..\generated\Win32\Panel\Debug /Y
    xcopy ..\..\ThirdParty\wxWidgets\generated\lib\vc_dll\wxmsw314ud_core_vc_custom.dll ..\..\generated\Win32\Panel\Debug /Y
    xcopy ..\..\ThirdParty\wxWidgets\generated\lib\vc_dll\wxbase314ud_vc_custom.dll ..\..\generated\Win32\Editor\Debug /Y
    xcopy ..\..\ThirdParty\wxWidgets\generated\lib\vc_dll\wxmsw314ud_core_vc_custom.dll ..\..\generated\Win32\Editor\Debug /Y
    goto EXIT

:HINT
    echo.
    echo Usage:
    echo       copy.bat [WX_DLLS]
    echo.
    goto EXIT

:EXIT
