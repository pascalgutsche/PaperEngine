@echo OFF
pushd %~dp0
call %PAPER_DIR%\premake\bin\premake5.exe vs2022
popd