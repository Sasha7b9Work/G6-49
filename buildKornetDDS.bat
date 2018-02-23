@echo off

@echo Build KornetDDS

echo Compile PanelDDS ... %TIME%

c:\Keil_v5\UV4\UV4 -b sources\Panel\PanelDDS.uvprojx -j0 -z -o PanelDDS.out
if ERRORLEVEL 1 goto BadPanelDDS

echo Compile Generator ... %TIME%

:FinePanelDDS
c:\Keil_v5\UV4\UV4 -b sources\Generator\Generator.uvprojx -j0 -z -o Generator.out
if ERRORLEVEL 1 goto BadGenerator

:FineGenerator
goto leave

:BadPanelDDS
echo ERROR!!! PanelDDS failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop sources\Panel\PanelDDS.out -n1000 -x0 -y0
goto FinePanelDDS

:BadGenerator
echo ERROR!!! Generator failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop sources\Generator\Generator.out -n1000 -x0 -y0
goto FineGenerator

:leave
