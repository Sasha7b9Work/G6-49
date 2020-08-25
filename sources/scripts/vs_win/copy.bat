@echo off

if "%1" EQU "" goto HINT
if %1==resources goto COPY_RESOURCES
if %1==WX_DLLS goto COPY_WX_DLLS
goto HINT

:COPY_RESOURCES
    @echo %TIME% Copying resources files
    mkdir ..\..\generated\Win32\Editor\icons
    mkdir ..\..\generated\Win32\Editor\Debug\icons
    xcopy ..\..\VS\Editor\icons ..\..\generated\Win32\Editor\Debug\icons /q /y
    xcopy ..\..\VS\Editor\icons ..\..\generated\Win32\Editor\icons /q /y
    goto EXIT

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
    echo       copy.bat [resources^|WX_DLLS]
    echo.
    goto EXIT

:EXIT
