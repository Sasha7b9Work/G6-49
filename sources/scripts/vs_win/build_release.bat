@echo off

cd ../..

rmdir "generated/Win32" /s /q

cd scripts/vs_win

cmake ../../VS/CMakeLists.txt -B../../generated/Win32 -DCMAKE_SYSTEM_VERSION=10.0.17763.0 -G "Visual Studio 16 2019" -A Win32 -DCONF_RELEASE=ON

MSBuild.exe ..\..\generated\Win32\G6-49_GUI.sln -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Succsess

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Succsess
@echo %TIME%   Complete

call copy_wxWidgets_dlls.bat
call copy_resources.bat

:Exit

