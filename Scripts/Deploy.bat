@echo off

SET destination=%1
SET version=%2

if [%destination%]==[] (SET /p destination="Enter deploy destination folder: ")
if [%version%]==[] (SET /p version="Enter version: ")

SET install=%destination%\Install\
SET archive=%destination%\%version%.zip

rmdir /s /q %install%
del %archive%

robocopy ../builds %install% *.exe *.dll
robocopy ../builds %install%/Libraries/NexusEngine *.lib

robocopy ../Assets %install%/Assets /e
robocopy ../Config %install%/Config /e
robocopy ../Libraries %install%/Libraries /e
robocopy ../Scripts %install%/Scripts /e 
robocopy ../Sources %install%/Sources /e /xf *.vcxproj *.vcxproj.user *.vcxproj.filters

powershell Compress-Archive %install% %archive%

pause