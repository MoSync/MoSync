REM @echo off

echo Compiling %1...

REM  2>&1 | sed 's/\([-a-zA-Z0-9/]\+\)\(.[-a-zA-Z0-9]\+\):\([0-9]\+\):/%CD:\=\/%\/\1\2(\3):/'
REM -Wshadow
xgcc -g -I%MOSYNCDIR%/include -I%MOSYNCDIR%/libs -S -O0 -fno-omit-frame-pointer -Wall -Werror -Wextra -Wno-unused-parameter -Wwrite-strings -o temp.s %1

if not exist temp.s goto end
echo Done.

IF E%ELIM%==Ejava GOTO JAVA
IF NOT E%ELIM%==EY GOTO REGULAR
pipe-tool -xerr -elim -B program_full temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib"
pipe-tool -xerr -B program rebuild.s
GOTO COPYFILES

:JAVA
pipe-tool -java -master-dump -B program temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib"
call prever.bat
GOTO end

:REGULAR
pipe-tool -xerr -master-dump -sld -collect-stabs -no-verify -B program temp.s "%MOSYNCDIR%\lib\pipe\mastdD.lib" "%MOSYNCDIR%\lib\pipe\mautilD.lib"

REM pipe-tool -elim -java -B program temp.s

:COPYFILES
call copyfiles.bat

:end
