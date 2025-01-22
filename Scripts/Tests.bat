@echo off

set Current=%cd%
set Root=%~dp0..\
cd %Root%
call %Root%builds\artifacts\NexusTests.exe
cd %Current%

if %errorlevel% == 0 exit /b else pause
