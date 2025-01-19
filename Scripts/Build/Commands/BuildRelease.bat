@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Release

if %errorlevel% == 0 exit /b else pause
