@echo off

@echo .
@echo %TIME%   Making Visual Studio Win Debug ...

cd ../..

rmdir "generated/Win32" /s /q

cd scripts/vs_win

@echo on
cmake ../../VS/CMakeLists.txt -B../../generated/Win32
@echo off

@echo %TIME%   Complete
