@echo off

cd keil
call build.bat all
cd ..

cd vs_keil
call assembly.bat build
cd ..

cd vs_win
call build.bat
cd ..
