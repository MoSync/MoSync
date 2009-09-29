REM @echo off
del program 2>nul
del resources 2>nul
del temp.s 2>nul
REM cls
REM echo Building %1...

if not exist %2.lst goto end
cd %1
pipe-tool -xerr -R ..\resources ..\%2.lst
cd ..

:end
