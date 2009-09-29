pipe-tool -dump-syms -B program temp.s "%MOSYNCDIR%\lib\pipe\mastd.lib" "%MOSYNCDIR%\lib\pipe\mautil.lib" > sym.lst
sort /+12 sym.lst /O symsort.lst
