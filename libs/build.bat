@REM	echo off

if exist "%MOSYNCDIR%\lib" goto step1
mkdir "%MOSYNCDIR%\lib"
:step1

if exist "%MOSYNCDIR%\lib\pipe" goto step2
mkdir "%MOSYNCDIR%\lib\pipe"
:step2
del /Q "%MOSYNCDIR%\lib\pipe\*.*"

if exist "%MOSYNCDIR%\include" goto step3
@echo error: run IDL first!
goto end
:step3

if exist "%MOSYNCDIR%\include\MAUtil" goto step4
mkdir "%MOSYNCDIR%\include\MAUtil"
:step4

if exist "%MOSYNCDIR%\include\MAUI" goto step5
mkdir "%MOSYNCDIR%\include\MAUI"
:step5

if exist "%MOSYNCDIR%\include\MTXml" goto step6
mkdir "%MOSYNCDIR%\include\MTXml"
:step6

@echo on

call copyHeaders.bat

cd mastd
mobuild -build mastd.mopro
if not %errorlevel%==0 goto end
@cd ..

call mautil_build.bat

call mtx_build.bat

call mafs_build.bat

call matest_build.bat

call maui_build.bat


:end
set PL_ERRORLEVEL=%errorlevel%
