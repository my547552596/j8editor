% m@yiyi 2018.9.29 %
@echo off

cd /d %~dp0
call ..\se.bat

for %%b in ("%cd%") do (
	windres %%~nb.rc -O coff %%~nb.res
    gcc -O2 -mwindows *.c -static ../bo/xSeal.a %%~nb.res -o %%~nb
	%%~nb
)

pause