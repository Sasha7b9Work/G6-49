@echo off

@echo Rebuild DDS

c:\Keil_v5\UV4\UV4 -c sources\Panel\PanelDDS.uvprojx -j0
c:\Keil_v5\UV4\UV4 -c sources\Generator\Generator.uvprojx -j0

call buildDDS.bat
