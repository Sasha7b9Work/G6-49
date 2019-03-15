@echo off
cd  vs_keil
call make.bat
cd ../vs_win
call make.bat
cd ..
call rebuild.bat
