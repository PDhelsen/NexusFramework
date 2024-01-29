@echo off

SET premake_file=%1
if [%premake_file%]==[] (SET /p premake_file="Enter premake file path: ")

cd Premake
call Premake5_Project.bat %premake_file%

pause