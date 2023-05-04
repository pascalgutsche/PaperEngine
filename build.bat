@echo OFF
echo Linking Project!
if exist premake5.lua (
	call premake\premake5.exe vs2022
) else (
	echo Please run setup.bat in the scripts folder first to set up project!
)
PAUSE
