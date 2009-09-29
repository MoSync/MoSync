@echo.

@IF "%1" == "" goto error
@IF "%2" == "" goto error
@goto START
:error
REM Usage: build_package.bat c:\SonyEricsson\JavaME_SDK_CLDC\PC_Emulation\wtk2 c:\Program\j2sdk1.4.2_10\bin 2>&1 | tee log.txt [/NOJAVA] [/SKIP]
@goto TOOL_ERROR
:START

@IF EXIST "%1" goto START2
@ECHO Directory "%1" could not be found.
@goto TOOL_ERROR
:START2

@SET MOSYNC_JAVA_SDK_DIR=%1

@REM For sed and grep on svn output
@SET LANG=EN

@FOR /F "TOKENS=1* DELIMS= " %%A IN ('DATE/T') DO @SET DATE=%%B
@FOR /F "TOKENS=*" %%A IN ('TIME/T') DO @SET TIME=%%A

@echo ------------------------------------------------
@echo Build started %TIME% %DATE%
@echo ------------------------------------------------
@echo.


@echo ------------------------------------------------
@echo Building package directory structure.
@echo ------------------------------------------------
@SET ORIGINAL_PATH=%CD%
@SET MOSYNC_RELEASE_BUILD_PATH=\mb
@SET MOSYNC_PATH=%MOSYNC_RELEASE_BUILD_PATH%\MoSyncReleasePackage
@SET MOSYNCDIR=%MOSYNC_PATH%
@SET MOSYNC_BIN_PATH=%MOSYNC_PATH%\bin
@SET MOSYNC_LIB_PATH=%MOSYNC_PATH%\lib
@SET MOSYNC_LIB_W32_PATH=%MOSYNC_PATH%\lib\w32
@SET MOSYNC_LIB_MOSYNC_PATH=%MOSYNC_PATH%\lib\pipe
@SET MOSYNC_INCLUDE_PATH=%MOSYNC_PATH%\include
@SET MOSYNC_DOCS_PATH=%MOSYNC_PATH%\docs
@SET MOSYNC_EXAMPLES_PATH=%MOSYNC_PATH%\examples
@SET MOSYNC_PROFILES_PATH=%MOSYNC_PATH%\profiles
@SET MOSYNC_TRUNK=%MOSYNC_RELEASE_BUILD_PATH%\mosync-trunk

@IF "%3" == "/SKIP" goto MKDIRS

@IF EXIST %MOSYNC_PATH% rmdir /s /q %MOSYNC_PATH%

:MKDIRS

@mkdir %MOSYNC_RELEASE_BUILD_PATH%
@mkdir %MOSYNC_PATH%
@mkdir %MOSYNC_BIN_PATH%
@mkdir %MOSYNC_LIB_PATH%
@mkdir %MOSYNC_LIB_W32_PATH%
@mkdir %MOSYNC_LIB_MOSYNC_PATH%
@mkdir %MOSYNC_INCLUDE_PATH%
@mkdir %MOSYNC_EXAMPLES_PATH%
@mkdir %MOSYNC_DOCS_PATH%

@echo ------------------------------------------------
@echo Setting build package vars
@echo ------------------------------------------------

@SET PATH=%CD%\build_package_tools\bin;%MOSYNC_BIN_PATH%;C:\Windows\system32;%2;%PATH%
@echo %PATH%

@echo ------------------------------------------------
@echo Setting visual c++ vars
@echo ------------------------------------------------
@call "%VS80COMNTOOLS%/vsvars32.bat"
@echo.

@SET INCLUDE=%CD%\build_package_tools\include;%INCLUDE%
@SET LIB=%CD%\build_package_tools\lib;%LIB%
@echo.
