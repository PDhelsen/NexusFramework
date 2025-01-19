@echo off

set binaries=%1
set artifacts=%~dp0..\..\builds\artifacts\
robocopy %binaries% %artifacts% *.lib *.dll *.exe /it /is /e /v

if %errorlevel% == 0 exit /b else pause
