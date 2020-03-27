@echo off
@echo %TIME%   Load firmware to Panel Loader ...

c:\Keil_v5\UV4\UV4 -f ..\..\PLoader\PLoader.uvprojx -j0 -o ..\scripts\Keil\out.txt
