@echo off

set Current=%cd%
set Root=%~dp0..\
cd %Root%
call %Root%builds\artifacts\NexusSandbox.exe
cd %Current%

if %errorlevel% NEQ 0 pause
