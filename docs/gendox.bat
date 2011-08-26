REM This file is called by the package building script.


set DOCSDIR=%CD%

call "%VS80COMNTOOLS%/vsvars32.bat"

set MOSYNC_DOXYGEN_OUTPUT_DIR=%MOSYNCDIR%\docs

copy release_notes.txt %MOSYNC_DOXYGEN_OUTPUT_DIR%\
@IF NOT errorlevel 0 goto TOOL_ERROR
copy changelog.txt %MOSYNC_DOXYGEN_OUTPUT_DIR%\
@IF NOT errorlevel 0 goto TOOL_ERROR


REM sh -c ./gendox.sh 2>&1 | tee log.txt


cd ..\libs

REM call copyHeaders.bat
ruby workfile.rb

copy Doxyfile %MOSYNCDIR%\include\

cd ..
set SRCDIR=%CD%

cd %MOSYNCDIR%\include\
%SRCDIR%\tools\ReleasePackageBuild\build_package_tools\bin\doxygen
cd %DOCSDIR%

