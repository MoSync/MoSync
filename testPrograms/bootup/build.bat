mkdir Output
%MOSYNCDIR%\bin\xgcc -g -o Output/main.s -S -O2 -Wno-unused-parameter -I%MOSYNCDIR%/include -I -DMAPIP main.cpp
%MOSYNCDIR%\bin\pipe-tool -elim -s%MOSYNCDIR%/lib/pipe -B Output/program_full Output/main.s  MAStd.lib
%MOSYNCDIR%\bin\pipe-tool -B Output/program rebuild.s
REM -entry=crt0_startup_basic
