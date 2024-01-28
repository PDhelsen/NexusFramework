@echo off
SET source=%1
SET destination=%source%../../builds
SET file=%2

robocopy ../../../../Softwares/NexusEngine/Install/ %destination% NexusApp.exe NexusEngine.dll NexusEditor.dll /it /is /v

robocopy %source% %destination% %file% /it /is /e /v
if %errorlevel% == 1 exit 0 else exit %errorlevel%