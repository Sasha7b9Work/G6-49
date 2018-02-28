del log.txt
"c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" Generator.vcxproj /Out log.txt /Build
if ERRORLEVEL 0 goto NiceResult

@echo ERROR!!!

:NiceResult
