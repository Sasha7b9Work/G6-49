del Debug\Panel.exe
del Panel.out

@echo Build Panel-VS      ... %TIME%

"c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" Panel.vcxproj /Out Panel.out /Rebuild

if exist Debug\Panel.exe goto Return

if exist "c:\Program Files\Notepad++\notepad++.exe" (
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop Panel.out -n1000 -x0 -y0
) else (
"c:\Program Files (x86)\Notepad++\notepad++.exe" -multiInst -alwaysOnTop Panel.out -n1000 -x0 -y0
)

:Return
