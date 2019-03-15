@echo off

@echo .
@echo %TIME%   Rebuild VS-GUI version ...

MSBuild.exe ..\..\generated\Win32\G6-49.sln -clp:ErrorsOnly;WarningsOnly -nologo -t:rebuild /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Success
@echo %TIME%   Complete

:Exit

call copy_wxWidgets_dlls.bat
