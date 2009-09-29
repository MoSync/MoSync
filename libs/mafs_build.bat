@echo off

if exist "%MOSYNCDIR%\lib" goto step2
@echo error: build MAStd and MAUtil first!
goto end
:step2

if exist "%MOSYNCDIR%\lib\pipe" goto step4
@echo error: build MAStd and MAUtil first!
goto end
:step4

if exist "%MOSYNCDIR%\include" goto step3
@echo error: build MAStd and MAUtil first!
goto end
:step3

if exist "%MOSYNCDIR%\include\MAFS" goto step5
mkdir "%MOSYNCDIR%\include\MAFS"
:step5

@echo on

@cd MAFS
@mobuild -build MAFS.mopro
copy *.h "%MOSYNCDIR%\include\MAFS\"

:end

cd ..
