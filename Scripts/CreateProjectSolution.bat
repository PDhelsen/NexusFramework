@echo off

SET folder=%1
if [%folder%]==[] (SET /p folder="Enter new project path: ")

cd Premake
call Premake5_Project.bat %folder%/Scripts/Premake/Premake5_Project.lua