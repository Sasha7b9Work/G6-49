@echo off

@echo Rebuild DDS          ... %TIME%

@echo Clean Panel ...

c:\Keil_v5\UV4\UV4 -c ..\Panel\Panel.uvprojx -j0

@echo Ok!

@echo Clean Device ...

c:\Keil_v5\UV4\UV4 -c ..\Device\Device.uvprojx -j0

@echo Ok!

call buildDDS.bat
