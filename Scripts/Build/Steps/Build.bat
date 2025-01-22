@echo off

set Root=%~dp0..\..\..\
set Configuration=%1
msbuild %Root%NexusFramework.sln /p:Configuration=%Configuration% /p:Platform=Win64

if %errorlevel% NEQ 0 pause
