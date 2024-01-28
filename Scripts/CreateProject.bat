@echo off

SET destination=%1
SET name=%2

if [%destination%]==[] (SET /p destination="Enter new project path: ")
if [%name%]==[] (SET /p name="Enter new project name: ")

robocopy . %destination%/%name%/Scripts /e /xf CreateProject.bat Premake5_Engine.lua Premake5_Engine.bat 

cd %destination%/%name%/

md Assets
md Config
md Libraries
md Resources
md Sources

cd Scripts/Premake/
call Premake5_Project.bat

pause