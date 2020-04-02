@echo off

call build_load_device.bat

call build_load_panel.bat

call build_load_ploader.bat

call build_load_dloader.bat

@echo %TIME%   Complete...