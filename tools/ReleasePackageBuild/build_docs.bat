@echo ------------------------------------------------
@echo Building docs.
@echo ------------------------------------------------

cd %MOSYNC_PATH%
SET FULLPATH_MOSYNCDIR=%CD%

cd %MOSYNC_TRUNK%\tools\DocbookIndexer\src\com\mobilesorcery\mosync\docbook
call buildrun.bat %ORIGINAL_PATH%\build_package_tools\bin\docbookParser-lib

mkdir %MOSYNC_PATH%\docs\
@REM cd ..\..\..\..
xcopy output %MOSYNC_PATH%\docs\ /E /Y /D

cd %ORIGINAL_PATH%

@echo.
