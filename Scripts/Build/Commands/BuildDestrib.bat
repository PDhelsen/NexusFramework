@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Distrib

if %errorlevel% == 0 exit /b else pause
