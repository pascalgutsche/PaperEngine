@echo OFF
if exist ..\premake5.lua (
	echo This project is already set up
	PAUSE
	exit
)

set /p "lname=Enter project name: "

SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
    SET STR=%lname%
:: Taking the rest of the string as it is as we don't need to change that.
    set "restStr=%STR:~1,20%"
:: Taking the first character of the string and doing it to upper case.
    set upper=
    set "str=%STR:~0,1%"
    for /f "skip=2 delims=" %%I in ('tree "\%str%"') do if not defined upper set "upper=%%~I"
    set "upper=%upper:~3%"
:: Printing the result by concating both of them.
set uname=%upper%%restStr%

echo %lname%
echo %uname%

echo Setting up project!

xcopy /e /k /h /i .\[game]\ ..\%lname%\
ren ..\%lname%\src\[Game].cpp %uname%.cpp
::powershell -Command "(Get-Content ..\%lname%\src\%uname%.cpp) -replace '[game]', '%lname%' | Out-File -encoding ASCII ..\%lname%\src\%uname%.cpp"
::powershell -Command "(Get-Content ..\%lname%\src\%uname%.cpp) -replace '[Game]', '%uname%' | Out-File -encoding ASCII ..\%lname%\src\%uname%.cpp"

PAUSE