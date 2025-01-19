@echo off
set Root=%~dp0..\

start "" git-gui.exe --working-dir %Root%
start "" %Root%NexusFramework.sln

if %errorlevel% == 0 exit /b else pause
