echo preprocessing %1/%2.jpp

@REM sed reformats gcc's error output so that Visual Studio can understand it

xgcc -x c -E -D_android -I%JAVASOURCEROOT%/shared -I%JAVASOURCEROOT%/platforms/android/src -o build/%2.jtmp %1/%2.jpp 2>&1 | sed "s/\([a-zA-Z/]\+\)\(.[a-zA-Z]\+\):\([0-9]\+\):/\1\2(\3):/"

@REM sed changes gcc's file position lines into comments
@REM the second sed changes source code filenames into numbers for the PUBLIC_DEBUG version
sed "s/^# /\/\//" < build/%2.jtmp > "%TARGETDIR%/%2.java"
@REM  | sed "s/"source\/%1.jpp"+":"/"%2-"/"
