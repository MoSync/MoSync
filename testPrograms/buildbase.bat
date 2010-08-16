REM @echo off

echo Compiling %1...

REM  2>&1 | sed 's/\([-a-zA-Z0-9/]\+\)\(.[-a-zA-Z0-9]\+\):\([0-9]\+\):/%CD:\=\/%\/\1\2(\3):/'
REM -Wshadow

IF E%NEWLIB%==EY GOTO USE_NEWLIB
set INCLUDE_POSTFIX=
set LIBDIR_PREFIX=pipe
set STDLIB=mastd
GOTO SET_FLAGS
:USE_NEWLIB
set INCLUDE_POSTFIX=/newlib
set LIBDIR_PREFIX=newlib
set STDLIB=newlib

:SET_FLAGS
set IFLAGS=-I%MOSYNCDIR%/include%INCLUDE_POSTFIX% -I%MOSYNCDIR%/libs -I%MOSYNCDIR%/profiles/vendors/MobileSorcery/Emulator
set CFLAGS=-finput-charset=UTF-8 -O0 -fno-omit-frame-pointer -Wall -Werror -Wextra -Wno-unused-parameter -Wwrite-strings
echo xgcc -g -S %IFLAGS% %CFLAGS% -o temp.s %1
xgcc -g -S %IFLAGS% %CFLAGS% -o temp.s %1
@REM gcj  -g -C -E %IFLAGS% %CFLAGS% %1
@REM goto end

if not exist temp.s goto end
echo Done.

IF E%ELIM%==Ejava GOTO JAVA
IF NOT E%ELIM%==EY GOTO REGULAR
pipe-tool -xerr -elim -B program_full temp.s "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_release\%STDLIB%.lib" "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_release\mautil.lib"
pipe-tool -xerr -B program rebuild.s
GOTO COPYFILES

:JAVA
@REM pipe-tool -xerr -elim -B program_full temp.s "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\%STDLIB%.lib" "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\mautil.lib"
pipe-tool -java "-gcj=\"--CLASSPATH=c:/Program/Research In Motion/BlackBerry JDE 5.0.0/lib/net_rim_api.jar;c:/SonyEricsson/JavaME_SDK_CLDC/PC_Emulation/WTK2/apps/JavaVM/classes/\"" -master-dump -B program temp.s "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\%STDLIB%.lib" "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\mautil.lib"
call prever.bat
GOTO end

:REGULAR
set CM=pipe-tool -stacksize=64000 -datasize=512000 -master-dump -sld=sld.tab -stabs=stabs.tab -no-verify -B program temp.s "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\%STDLIB%.lib" "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\mautil.lib" "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\maui.lib" "%MOSYNCDIR%\lib\%LIBDIR_PREFIX%_debug\mtxml.lib"
echo %CM%
%CM%

REM pipe-tool -elim -java -B program temp.s

:COPYFILES
call copyfiles.bat

:end
