@echo off

@echo .
@echo %TIME%   Build G6-49

call build_device.bat

call build_dloader.bat

call build_panel.bat

call build_ploader.bat

@echo %TIME%   Complete
