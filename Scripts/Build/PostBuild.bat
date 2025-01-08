@echo off
SET source=%1
SET destination=%source%../../builds

robocopy %source% %destination% *.lib *.dll *.exe /it /is /e /v
if %errorlevel% == 1 exit 0 else exit %errorlevel%