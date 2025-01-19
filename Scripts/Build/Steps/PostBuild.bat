@echo off

set Root=%~dp0..\..\..\
set Binaries=%1
set Artifacts=%Root%builds\artifacts\
robocopy %Binaries% %Artifacts% *.lib *.dll *.exe /it /is /e /v

if %errorlevel% == 0 exit /b else pause
