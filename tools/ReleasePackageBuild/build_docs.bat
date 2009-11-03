
cd %MOSYNC_PATH%
SET FULLPATH_MOSYNCDIR=%CD%

@echo ------------------------------------------------
@echo Building documentation - Package
@echo ------------------------------------------------

cd %MOSYNC_TRUNK%\tools\DocbookIndexer\src\com\mobilesorcery\mosync\docbook
call buildrun.bat %ORIGINAL_PATH%\build_package_tools\bin\docbookParser-lib

mkdir %MOSYNC_PATH%\docs\
xcopy output %MOSYNC_PATH%\docs\ /E /Y /D

@echo ------------------------------------------------
@echo Building documentation - Eclipse
@echo ------------------------------------------------
cd %MOSYNC_TRUNK%\tools\DocbookIndexer\src\com\mobilesorcery\mosync\docbook
call buildrun.bat %ORIGINAL_PATH%\build_package_tools\bin\docbookParser-lib eclipse

mkdir %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.help\docs\
xcopy output %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.help\docs\ /E /Y /D

cd %ORIGINAL_PATH%

@echo.
