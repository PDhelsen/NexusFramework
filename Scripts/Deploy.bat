@echo off
setlocal EnableDelayedExpansion

set Root=%~dp0..\

set Target=%1
if [%Target%] == [] (set /p Target=Enter target directory:)

set Deploy=%Target%\NexusFramework\
if exist %Deploy% rmdir /s /q %Deploy%
mkdir %Deploy%

set FolderLastIndex=1
set Folders[0]=NexusFramework
set Folders[1]=NexusUtility

call :CopyFolder %Root%Sources %Deploy%Sources "*.h *.cpp *.natvis"
call :CopyFolder %Root%builds\binaries %Deploy%Builds "*.dll *.lib *.pdb"

if errorlevel 1 (pause) else (exit /b 0)

::------------------------------------------------
:CopyFolder
setlocal

for /d %%F in (%1\*) do (
	call :ContainsSubstring %%~nxF
	if !Result!==true (
		robocopy %%F %2\%%~nxF %~3 /e /s
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
