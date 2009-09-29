@echo off

call buildprep.bat %1 %1\main

if not exist MAHeaders.h goto bb
move MAHeaders.h %1

:bb
if not exist %1\main.cpp goto c
buildbase %1\main.cpp
:c
buildbase %1\main.c
