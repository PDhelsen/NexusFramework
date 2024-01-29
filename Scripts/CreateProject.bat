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

@REM Create Project Solution
robocopy Premake %destination%/%name%/Scripts/Premake Premake5_Project.lua
call CreateProjectSolution.bat %destination%/%name%/

pause