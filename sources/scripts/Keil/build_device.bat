@echo off
@echo %TIME%   Compile Device ...

c:\Keil_v5\UV4\UV4 -b ..\..\Device\Device.uvprojx -j0 -z -o Device.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Device    failed!!!

type ..\..\Device\DeviceOSC.out

:Exit

copy ..\..\Device\S8-57.bin h:\
copy ..\..\Device\S8-57.bin i:\

@echo %TIME%   Complete
