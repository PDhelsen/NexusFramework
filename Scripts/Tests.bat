@echo off

set Current=%cd%
set Root=%~dp0..\
cd %Root%
call %Root%builds\artifacts\NexusTests.exe
set Result=%errorlevel%
cd %Current%

if %Result% 1 (pause) else (exit /b 0)
if errorlevel 1 (pause) else (exit /b 0)
