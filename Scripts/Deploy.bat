@echo off

set Root=%~dp0..\

set Target=%1
if [%Target%] == [] (set /p Target=Enter target directory:)

set Deploy=%Target%\NexusFramework\
if exist %Deploy% rmdir /s /q %Deploy%
mkdir %Deploy%

robocopy %Root%Sources\NexusFramework\ %Deploy%Sources\NexusFramework\ *.h *.cpp *.natvis /e

for /d %%F in (%Root%builds\binaries\*) do (
    echo %%~nxF | findstr /i NexusFramework > nul
    if not errorlevel 1 (
        robocopy %%F %Deploy%Builds\%%~nxF /e
    )
)

if %errorlevel% NEQ 0 pause
