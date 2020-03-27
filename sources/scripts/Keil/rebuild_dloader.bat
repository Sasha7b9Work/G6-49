echo off
@echo %TIME%   Rebuild Device Loader

call clean_dloader.bat

call build_dloader.bat
