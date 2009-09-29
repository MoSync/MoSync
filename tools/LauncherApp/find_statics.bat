rem ------ start of find_statics.bat ------

@echo off

for /R %%f in (*.o) do ( echo %%f & nm %%f > tmp.txt & find /I " d " < tmp.txt & find /I " b " < tmp.txt & find /I " g " < tmp.txt )

del /f tmp.txt 2> NUL

rem ------ end of find_statics.bat ------
