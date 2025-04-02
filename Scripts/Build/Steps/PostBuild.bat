@echo off

set Root=%~dp0..\..\..\
set Artifacts=%Root%builds\artifacts\
set Binaries=%1
robocopy %Binaries% %Artifacts% *.lib *.dll *.exe /it /is /e /v

if errorlevel 1 (pause) else (exit /b 0)
