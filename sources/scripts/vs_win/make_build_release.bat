@echo off

cd ../..

rmdir "generated/Win32" /s /q

cd scripts/vs_win

cmake ../../VS/CMakeLists.txt -B../../generated/Win32 -G "Visual Studio 16 2019" -A Win32 -DBUILD_USE_STATIC_RUNTIME=ON

MSBuild.exe ..\..\generated\Win32\G6-49_GUI.sln -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Succsess

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Succsess
@echo %TIME%   Complete

call copy.bat WX_DLLS
call copy.bat resources

:Exit

