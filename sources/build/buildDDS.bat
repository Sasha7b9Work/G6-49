@echo off

@echo Build DDS

@echo Build Panel DDS  ... %TIME%

c:\Keil_v5\UV4\UV4 -b ..\Panel\Panel.uvprojx -j0 -z -o Panel.out
if ERRORLEVEL 1 goto BadPanelDDS

@echo Ok!

@echo Build Device DDS ... %TIME%

:FinePanelDDS
c:\Keil_v5\UV4\UV4 -b ..\Device\Device.uvprojx -j0 -z -o Device.out
if ERRORLEVEL 1 goto BadDeviceDDS

@echo Ok!

:FineDeviceDDS
goto leave

:BadPanelDDS
echo ERROR!!! Panel DDS failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\Panel\Panel.out -n1000 -x0 -y0
goto FinePanelDDS

:BadDeviceDDS
echo ERROR!!! Device DDS failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\Device\Device.out -n1000 -x0 -y0
goto FineDeviceDDS

:leave
