@echo off
premake5 vs2022 --file=%~dp0Solution.lua --verbose
if %errorlevel% == 0 exit /b else pause
