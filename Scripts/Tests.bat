@echo off

set Current=%cd%
set Root=%~dp0..\
cd %Root%
call %Root%builds\artifacts\NexusTests.exe
set Result=%errorlevel%
cd %Current%

if %Result% NEQ 0 pause
if %errorlevel% NEQ 0 pause
