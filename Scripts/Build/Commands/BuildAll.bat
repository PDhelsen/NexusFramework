@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Debug
call %Root%Scripts\Build\Steps\Build.bat Distrib
call %Root%Scripts\Build\Steps\Build.bat Release
