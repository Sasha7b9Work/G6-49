@echo off
cd keil
call build.bat all
cd ../vs_keil
call make.bat build
cd ../vs_win
call make.bat build
cd ..
