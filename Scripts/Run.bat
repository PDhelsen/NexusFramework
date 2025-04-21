@echo off

set Root=%~dp0..\

pushd %Root%
call %Root%builds\artifacts\NexusSandbox.exe %*
popd

if errorlevel 1 (pause) else (exit /b 0)

