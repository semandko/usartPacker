@echo off&&cls

del out.txt
cd. > out.txt

set arg1=%1

REM check argument %1 if empty goto error
if "%1" EQU "" (
	ECHO use keys: clean, make
	goto end
)

if %arg1% == clean (
	ECHO command = clean
	mingw32-make clean
	goto end
)

if %arg1% == make (
	ECHO command = make
	mingw32-make >out.txt 2>error.txt
	pause
	goto end
)

ECHO command = %arg1% is wrong

:end
