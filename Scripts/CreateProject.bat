@echo off

SET destination=%1
SET name=%2

if [%destination%]==[] (SET /p destination="Enter new project path: ")
if [%name%]==[] (SET /p name="Enter new project name: ")

@REM Create Hierarchy
pushd %destination%
md %name%
cd %name%
md Assets
md Config
md Libraries
md Resources
md Scripts
md Sources
popd

@REM Copy File
robocopy ../Config %destination%/%name%/Config DllExport.def
robocopy Premake %destination%/%name%/Scripts/Premake Premake5_Project.lua

@REM Create Project Solution
call CreateProjectSolution.bat %destination%/%name%/Scripts/Premake/Premake5_Project.lua

pause