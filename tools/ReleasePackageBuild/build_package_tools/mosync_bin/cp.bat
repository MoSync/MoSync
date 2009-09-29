@echo off
set SRC=%1
set DST=%2
set SRC=%SRC:/=\%
set DST=%DST:/=\%
copy /Y %SRC% %DST%
