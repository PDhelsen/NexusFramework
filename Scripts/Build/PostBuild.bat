@echo off
SET source=%1
SET destination=%source%../../artifacts

robocopy %source% %destination% *.lib *.dll *.exe /it /is /e /v
if %errorlevel% == 0 exit /b else pause
