REM Copyright (C) 2009 Mobile Sorcery AB

REM This program is free software; you can redistribute it and/or modify it under
REM the terms of the GNU General Public License, version 2, as published by
REM the Free Software Foundation.

REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
REM or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
REM for more details.

REM You should have received a copy of the GNU General Public License
REM along with this program; see the file COPYING.  If not, write to the Free
REM Software Foundation, 59 Temple Place - Suite 330, Boston, MA
REM 02111-1307, USA.

@echo build_package.bat
@echo.

@IF "%1" == "" goto error

@goto START
:error
REM Usage: build_package.bat c:\SonyEricsson\JavaME_SDK_CLDC\OnDeviceDebug\ 2>&1 | tee log.txt [/NOJAVA] [/SKIP]
@goto TOOL_ERROR
:START

@ECHO MOSYNC_RUNTIME_HAVE = %MOSYNC_RUNTIME_HAVE%

@IF EXIST "%1" goto START2
@ECHO Directory "%1" could not be found.
@goto TOOL_ERROR
:START2

@SET JAVAMESDKDIR=%1

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
@SET ECLIPSE_TRUNK=%MOSYNC_RELEASE_BUILD_PATH%\eclipse
@SET MOSYNC_PATH=%MOSYNC_RELEASE_BUILD_PATH%\MoSyncReleasePackage
@SET MOSYNCDIR=%MOSYNC_PATH%
@SET MOSYNC_BIN_PATH=%MOSYNC_PATH%\bin
@SET MOSYNC_ETC_PATH=%MOSYNC_PATH%\etc
@SET MOSYNC_ECLIPSE_PATH=%MOSYNC_PATH%\eclipse
@SET MOSYNC_LIB_PATH=%MOSYNC_PATH%\lib
@SET MOSYNC_LIB_W32_PATH=%MOSYNC_PATH%\lib\w32
@SET MOSYNC_LIB_MOSYNC_PATH=%MOSYNC_PATH%\lib\pipe
@SET MOSYNC_INCLUDE_PATH=%MOSYNC_PATH%\include
@SET MOSYNC_DOCS_PATH=%MOSYNC_PATH%\docs
@SET MOSYNC_EXAMPLES_PATH=%MOSYNC_PATH%\examples
@SET MOSYNC_PROFILES_PATH=%MOSYNC_PATH%\profiles
@SET MOSYNC_MAPIP_BIN_PATH=%MOSYNC_PATH%\mapip\bin
@SET MOSYNC_TRUNK=%MOSYNC_RELEASE_BUILD_PATH%\mosync-trunk

@IF "%2" == "/SKIP" goto MKDIRS

@IF EXIST %MOSYNC_PATH% rmdir /s /q %MOSYNC_PATH%

:MKDIRS

@mkdir %MOSYNC_RELEASE_BUILD_PATH%
@mkdir %MOSYNC_PATH%
@mkdir %MOSYNC_BIN_PATH%
@mkdir %MOSYNC_ETC_PATH%
@mkdir %MOSYNC_ECLIPSE_PATH%
@mkdir %MOSYNC_LIB_PATH%
@mkdir %MOSYNC_LIB_W32_PATH%
@mkdir %MOSYNC_LIB_MOSYNC_PATH%
@mkdir %MOSYNC_INCLUDE_PATH%
@mkdir %MOSYNC_EXAMPLES_PATH%
@mkdir %MOSYNC_DOCS_PATH%
@mkdir %MOSYNC_MAPIP_BIN_PATH%

@echo ------------------------------------------------
@echo Setting build package vars
@echo ------------------------------------------------

@SET PATH=%CD%\build_package_tools\bin;%MOSYNC_BIN_PATH%;%JAVA_SDK_BIN%;%PATH%
@echo PATH:
@echo %PATH%

@echo ------------------------------------------------
@echo Setting visual c++ vars
@echo ------------------------------------------------
@call "%VS100COMNTOOLS%/vsvars32.bat"
@echo.

@SET INCLUDE=%CD%\build_package_tools\include;%CD%\build_package_tools\include\msvc;%INCLUDE%
@SET LIB=%CD%\build_package_tools\lib;%LIB%
@echo.

@echo ------------------------------------------------
@echo Testing sed
@echo ------------------------------------------------

set
sed -v
@echo.

@IF "%2" == "/SKIP" goto COPY

cd %ORIGINAL_PATH%

:MK_TRUNK_DIRS
@mkdir %MOSYNC_TRUNK%\include
@mkdir %MOSYNC_TRUNK%\lib
@mkdir %MOSYNC_TRUNK%\lib\pipe
@echo.

:COPY
@echo off

@echo.
@echo ------------------------------------------------
@echo Copying MoSync bin.
@echo ------------------------------------------------
@xcopy %ORIGINAL_PATH%\build_package_tools\mosync_bin %MOSYNC_BIN_PATH% /y /E /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@echo.
@echo ------------------------------------------------
@echo Copying MAPIP bin.
@echo ------------------------------------------------
@xcopy %ORIGINAL_PATH%\build_package_tools\mapip_bin %MOSYNC_MAPIP_BIN_PATH% /y /E /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@echo.
@echo ------------------------------------------------
@echo Copying Batik bin.
@echo ------------------------------------------------
@xcopy %ORIGINAL_PATH%\build_package_tools\bin\Batik %MOSYNC_BIN_PATH%\Batik\ /y /E /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@echo.
@echo ------------------------------------------------
@echo Copying skins.
@echo ------------------------------------------------
@xcopy %MOSYNC_TRUNK%\skins %MOSYNC_PATH%\skins\ /y /E /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
@echo.

@echo.
@echo ------------------------------------------------
@echo Copying rules.
@echo ------------------------------------------------
@xcopy %MOSYNC_TRUNK%\rules %MOSYNC_PATH%\rules\ /y /E /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
@echo.

@xcopy %MOSYNC_TRUNK%\MoSyncRules.rules %MOSYNC_PATH%\ /y /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
@echo.

@echo ------------------------------------------------
@echo Copying Android icon and keystore
@echo ------------------------------------------------
@xcopy %MOSYNC_TRUNK%\runtimes\java\platforms\android\AndroidProject\res\drawable\icon.png %MOSYNC_ETC_PATH%\ /y /D
@xcopy %MOSYNC_TRUNK%\runtimes\java\platforms\android\default.icon %MOSYNC_ETC_PATH%\ /y /D
@xcopy %MOSYNC_TRUNK%\runtimes\java\platforms\android\mosync.keystore %MOSYNC_ETC_PATH%\ /y /D

@echo ------------------------------------------------
@echo Copying example contacts.xml
@echo ------------------------------------------------
@copy %MOSYNC_TRUNK%\runtimes\cpp\platforms\sdl\contacts.xml %MOSYNC_BIN_PATH%\default_contacts.xml /y /D

@echo ------------------------------------------------
@echo Building Mosync.
@echo ------------------------------------------------
@cd %MOSYNC_TRUNK%\
ruby workfile.rb
ruby workfile.rb CONFIG=
ruby workfile.rb USE_NEWLIB=
ruby workfile.rb CONFIG= USE_NEWLIB=
@echo.



@cd %MOSYNC_TRUNK%\libs
@call copyHeaders.bat

@echo ------------------------------------------------
@echo Building examples (Win32)
@echo ------------------------------------------------
@xcopy %MOSYNC_TRUNK%\examples %MOSYNC_EXAMPLES_PATH% /e /y

@REM	Get rid of examples that are not supposed to be in this release
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\MAXml
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\MoChip
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\MoRaw
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\MoVec
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\flashback
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\MAUI\clock
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\MAUI\multi
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\wolf3d
@rmdir /s /q %MOSYNC_EXAMPLES_PATH%\HelloLayerMap

@REM Remove files that shouldn't be in the release
@del /s /q %MOSYNC_EXAMPLES_PATH%\Makefile* %MOSYNC_EXAMPLES_PATH%\*.bat %MOSYNC_EXAMPLES_PATH%\*.2008.vcproj

@echo ------------------------------------------------
@echo Building docs.
@echo ------------------------------------------------
:DOCS

cd %ORIGINAL_PATH%

@xcopy %ORIGINAL_PATH%\build_package_tools\mosync_docs %MOSYNC_DOCS_PATH% /e /y
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@cd %MOSYNC_TRUNK%\docs\
@echo on

call gendox.bat

@echo ------------------------------------------------
@echo Copies the doxygen html files to Eclipse
@echo ------------------------------------------------

@cd %MOSYNC_TRUNK%\docs\

mkdir %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.help\docs\html\
@xcopy %MOSYNC_DOCS_PATH%\html %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.help\docs\html\ /e /y
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@cd %MOSYNC_TRUNK%\tools\doxy2cdt\
ruby main.rb -b com.mobilesorcery.sdk.help/docs/html/ %MOSYNC_DOCS_PATH%\xml\index.xml %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.help\docs\apireference.xml
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@rd /s /q %MOSYNC_DOCS_PATH%\xml

:BUILD_ECLIPSE
@echo.
@echo ------------------------------------------------
@echo Building MoSync-Eclipse
@echo ------------------------------------------------
cd %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.product\build\

call build-mosync.bat release
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@echo.
@echo ------------------------------------------------
@echo Moving MoSync-Eclipse
@echo ------------------------------------------------
echo on
@cd %ECLIPSE_TRUNK%\com.mobilesorcery.sdk.product\build\
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@del /S /Q %MOSYNC_ECLIPSE_PATH%

xcopy buildresult\I.MoSync\MoSync-win32.win32.x86-unzipped\mosync %MOSYNC_ECLIPSE_PATH% /y /E /D
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR


@echo ------------------------------------------------
@echo Building Eclipse PanicDoc.
@echo ------------------------------------------------
@cd %MOSYNC_TRUNK%\tools\PanicDoc
@Release\PanicDoc.exe -props > %MOSYNC_ECLIPSE_PATH%\paniccodes.properties
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
@echo.

@cd %MOSYNC_RELEASE_BUILD_PATH%
@echo.
@echo ------------------------------------------------
@echo Building runtimes.
@echo BE PATIENT! This takes a while!
@echo ------------------------------------------------

REM @cd %MOSYNC_TRUNK%\tools\profileConverter
REM @mkdir %MOSYNC_PROFILES_PATH%
REM @call ruby conv.rb -dst %MOSYNC_PROFILES_PATH%
REM @IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

REM @cd %MOSYNC_TRUNK%\tools\ReleasePackageBuild
REM @call ruby buildRuntimes.rb
REM @IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR


@echo ------------------------------------------------
@echo Building PIPE libs
@echo ------------------------------------------------

:LIBS_BUILD

cd %MOSYNC_TRUNK%\libs

@call ruby workfile.rb
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@call ruby workfile.rb CONFIG=""
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@call ruby workfile.rb USE_NEWLIB=""
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@call ruby workfile.rb USE_NEWLIB="" CONFIG=""
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

REM del %MOSYNC_INCLUDE_PATH%\IX_*.h
REM @IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

REM @rmdir /s /q %MOSYNC_INCLUDE_PATH%\GLES


@echo ------------------------------------------------
@echo Copying MoBuild templates
@echo ------------------------------------------------
mkdir %MOSYNC_PATH%\templates
@xcopy %MOSYNC_TRUNK%\templates %MOSYNC_PATH%\templates /E /y
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
@echo.

@echo ------------------------------------------------
@echo Making installation package.
@echo ------------------------------------------------
echo on
xcopy %ORIGINAL_PATH%\InstallerResources\*.* %MOSYNCDIR% /y
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
xcopy %ORIGINAL_PATH%\build_package_tools\mosync_prerequisites\*.* %MOSYNCDIR% /y
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
copy %ORIGINAL_PATH%\build_package_tools\mosync_docs\licenses\mosync-license.txt %MOSYNCDIR%\license.txt /y
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR
REM ren %MOSYNCDIR%\docs\000_index.html index.html
REM @IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

%ORIGINAL_PATH%\build_package_tools\NSIS\makensis %MOSYNC_PATH%\MoSync.nsi
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

ren %MOSYNCDIR%\MoSyncSetup.exe MoSyncSetup.exe

@del %MOSYNC_PATH%\MoSync.nsi
@del %MOSYNC_PATH%\installer.bmp
@del %MOSYNC_PATH%\installer_splash.bmp
@del %MOSYNC_PATH%\icon.ico
@del %MOSYNC_PATH%\license.txt
@echo.

:END_BUILD

@FOR /F "TOKENS=1* DELIMS= " %%A IN ('DATE/T') DO @SET DATE=%%B
@FOR /F "TOKENS=*" %%A IN ('TIME/T') DO @SET TIME=%%A
@echo ------------------------------------------------
@echo "Build ended:" %TIME% %DATE%
@echo ------------------------------------------------
@echo.

@goto END
:TOOL_ERROR
@echo a tool related error occured.
exit /b 23
:END

cd %ORIGINAL_PATH%
exit /b 0
