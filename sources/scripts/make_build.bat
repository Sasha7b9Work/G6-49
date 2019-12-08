@echo off
cd keil
call build.bat
cd ../vs_keil
call make_build.bat
cd ../vs_win
call make_build.bat
cd ..
