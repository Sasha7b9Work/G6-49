@echo off

@echo .
@echo %TIME%   Making Visual Studio Keil Debug ...

cd ../..
rmdir "generated/Keil" /s /q
cd scripts/vs_keil
@echo on
cmake ../../CMakeLists.txt -B../../generated/Keil
@echo off

@echo %TIME%   Complete
