@echo off

SET destination=%1
SET version=%2

if [%destination%]==[] (SET /p destination="Enter deploy destination folder: ")
if [%version%]==[] (SET /p version="Enter version: ")

SET install=%destination%\Install\
SET archive=%destination%\%version%.zip

rmdir /s /q %install%
del %archive%

robocopy ../builds %install%
robocopy ../Assets %install%/Assets /e
robocopy ../Config %install%/Config /e
robocopy ../Scripts %install%/Scripts /e /xf Deploy.bat PostBuildCommandsEngine.bat 
robocopy ../Sources %install%/Sources /e /xf *.vcxproj *vcxproj.user

powershell Compress-Archive %install% %archive%

pause