@echo off
setlocal EnableDelayedExpansion

set Root=%~dp0..\

set Target=%1
if [%Target%] == [] (set /p Target=Enter target directory:)

set Deploy=%Target%\NexusFramework\
if exist %Deploy% rmdir /s /q %Deploy%
mkdir %Deploy%

set FolderLastIndex=3
set Folders[0]=NexusFramework
set Folders[1]=NexusUtility
set Folders[2]=gtest
set Folders[3]=yaml-cpp

call :CopyFolder %Root%Libraries %Deploy%Libraries "*.h *.hpp *.cpp *.cc *.natvis"
call :CopyFolder %Root%Sources %Deploy%Sources "*.h *.cpp *.natvis"
call :CopyFolder %Root%builds\binaries %Deploy%Builds "*.dll *.lib *.pdb"

if errorlevel 1 (
	pause
) else (
	choice /M "Do you want to save %Deploy% to the NexusFramework environment variable?"
	if %errorlevel%==1 (
		setx NexusFramework %Target%\NexusFramework
	)
	
	exit /b 0
)

::------------------------------------------------
:CopyFolder
setlocal

for /d %%F in (%1\*) do (
	call :ContainsSubstring %%~nxF
	if !Result!==true (
		robocopy %%F %2\%%~nxF %~3 /it /is /e /v
	)
)

endlocal
exit /b 0

::------------------------------------------------
:ContainsSubstring
setlocal
set "Contains=false"

for /L %%I in (0,1,%FolderLastIndex%) do (
	set "Folder=!Folders[%%I]!"
	echo %1 | findstr /i /c:"!Folder!" > nul
	if not errorlevel 1 (
		set "Contains=true"
	)
)

(endlocal & set "Result=%Contains%")
exit /b 0
