REM @echo off

echo Compiling %1...

REM  2>&1 | sed 's/\([-a-zA-Z0-9/]\+\)\(.[-a-zA-Z0-9]\+\):\([0-9]\+\):/%CD:\=\/%\/\1\2(\3):/'
REM -Wshadow

set IFLAGS=-I%MOSYNCDIR%/include -I%MOSYNCDIR%/libs -I%MOSYNCDIR%/profiles/vendors/MobileSorcery/Emulator
set CFLAGS=-O0 -fno-omit-frame-pointer -Wall -Werror -Wextra -Wno-unused-parameter -Wwrite-strings
xgcc -g -S %IFLAGS% %CFLAGS% -o temp.s %1
@REM gcj  -g -C -E %IFLAGS% %CFLAGS% %1
@REM goto end

if not exist temp.s goto end
echo Done.

IF E%ELIM%==Ejava GOTO JAVA
IF NOT E%ELIM%==EY GOTO REGULAR
pipe-tool -xerr -elim -B program_full temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib"
pipe-tool -xerr -B program rebuild.s
GOTO COPYFILES

:JAVA
@REM pipe-tool -xerr -elim -B program_full temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib"
pipe-tool -java "-gcj=\"--CLASSPATH=c:/Program/Research In Motion/BlackBerry JDE 5.0.0/lib/net_rim_api.jar;c:/SonyEricsson/JavaME_SDK_CLDC/PC_Emulation/WTK2/apps/JavaVM/classes/\"" -master-dump -B program temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib"
call prever.bat
GOTO end

:REGULAR
pipe-tool -xerr -master-dump -sld -collect-stabs -no-verify -B program temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib" "%MOSYNCDIR%\lib\pipe\mauiD.lib"

REM pipe-tool -elim -java -B program temp.s

:COPYFILES
call copyfiles.bat

:end
