@echo off

@echo .
@echo %TIME%   Rebuild G6-49

call rebuild_device.bat

call rebuild_dloader.bat

call rebuild_panel.bat

call rebuild_ploader.bat

@echo %TIME%   Complete
