@echo off

set Root=%~dp0..\

pushd %Root%
call %Root%builds\artifacts\NexusTests.exe
set Result=%errorlevel%
popd %Current%

if %Result% 1 (pause) else (exit /b 0)
if errorlevel 1 (pause) else (exit /b 0)
