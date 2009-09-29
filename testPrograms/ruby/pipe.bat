call res.bat

c:\MoSync/bin/xgcc -o c:\mosync-trunk\testPrograms\ruby\Output\inits.s -S -Wno-unused-parameter -Wall -Ic:\MoSync\include -Ic:\mosync-trunk\testPrograms\ruby -Ic:\mosync-trunk\testPrograms\ruby\helpers -DMAPIP -Wno-unused-variable -Wno-unused-function c:\mosync-trunk\testPrograms\ruby\src\inits.c

c:\MoSync/bin/xgcc -o c:\mosync-trunk\testPrograms\ruby\Output\io.s -S -Wno-unused-parameter -Wall -Ic:\MoSync\include -Ic:\mosync-trunk\testPrograms\ruby -Ic:\mosync-trunk\testPrograms\ruby\helpers -DMAPIP -Wno-unused-variable -Wno-unused-function c:\mosync-trunk\testPrograms\ruby\src\io.c

REM c:\MoSync/bin/xgcc -o c:\mosync-trunk\testPrograms\ruby\Output\eval.s -S -Wno-unused-parameter -Wall -Ic:\MoSync\include -Ic:\mosync-trunk\testPrograms\ruby -Ic:\mosync-trunk\testPrograms\ruby\helpers -DMAPIP -Wno-unused-variable -Wno-unused-function c:\mosync-trunk\testPrograms\ruby\src\eval.c

c:\MoSync/bin/xgcc -o c:\mosync-trunk\testPrograms\ruby\Output\mosync.s -S -Wno-unused-parameter -Wall -Ic:\MoSync\include -Ic:\mosync-trunk\testPrograms\ruby -Ic:\mosync-trunk\testPrograms\ruby\helpers -DMAPIP -Wno-unused-variable -Wno-unused-function c:\mosync-trunk\testPrograms\ruby\mosync.c

c:\MoSync/bin/xgcc -o c:\mosync-trunk\testPrograms\ruby\Output\syscalls.s -S -Wno-unused-parameter -Wall -Ic:\MoSync\include -Ic:\mosync-trunk\testPrograms\ruby -Ic:\mosync-trunk\testPrograms\ruby\helpers -DMAPIP -Wno-unused-variable -Wno-unused-function c:\mosync-trunk\testPrograms\ruby\syscalls.c

pipe-tool -datasize=1048576 -heapsize=524288 -stacksize=131072 -master-dump -sc:\MoSync\lib\pipe -B c:\mosync-trunk\testPrograms\ruby\Output/program c:\mosync-trunk\testPrograms\ruby\Output\array.s  c:\mosync-trunk\testPrograms\ruby\Output\bignum.s  c:\mosync-trunk\testPrograms\ruby\Output\class.s  c:\mosync-trunk\testPrograms\ruby\Output\compar.s  c:\mosync-trunk\testPrograms\ruby\Output\enum.s  c:\mosync-trunk\testPrograms\ruby\Output\error.s  c:\mosync-trunk\testPrograms\ruby\Output\eval.s  c:\mosync-trunk\testPrograms\ruby\Output\gc.s  c:\mosync-trunk\testPrograms\ruby\Output\hash.s  c:\mosync-trunk\testPrograms\ruby\Output\inits.s  c:\mosync-trunk\testPrograms\ruby\Output\io.s  c:\mosync-trunk\testPrograms\ruby\Output\main.s  c:\mosync-trunk\testPrograms\ruby\Output\numeric.s  c:\mosync-trunk\testPrograms\ruby\Output\object.s  c:\mosync-trunk\testPrograms\ruby\Output\parse.s  c:\mosync-trunk\testPrograms\ruby\Output\prec.s  c:\mosync-trunk\testPrograms\ruby\Output\random.s  c:\mosync-trunk\testPrograms\ruby\Output\range.s  c:\mosync-trunk\testPrograms\ruby\Output\rb_e.s  c:\mosync-trunk\testPrograms\ruby\Output\re.s  c:\mosync-trunk\testPrograms\ruby\Output\regex.s  c:\mosync-trunk\testPrograms\ruby\Output\ruby.s  c:\mosync-trunk\testPrograms\ruby\Output\sprintf.s  c:\mosync-trunk\testPrograms\ruby\Output\st.s  c:\mosync-trunk\testPrograms\ruby\Output\string.s  c:\mosync-trunk\testPrograms\ruby\Output\struct.s  c:\mosync-trunk\testPrograms\ruby\Output\time.s  c:\mosync-trunk\testPrograms\ruby\Output\util.s  c:\mosync-trunk\testPrograms\ruby\Output\variable.s  c:\mosync-trunk\testPrograms\ruby\Output\version.s  c:\mosync-trunk\testPrograms\ruby\Output\file.s  c:\mosync-trunk\testPrograms\ruby\Output\mosync.s  c:\mosync-trunk\testPrograms\ruby\Output\alloca.s c:\mosync-trunk\testPrograms\ruby\Output\syscalls.s MAStd.lib 

copy /Y Output\program \mosync-trunk\runtimes\cpp\platforms\sdl\MoRE\
copy /Y Output\resources \mosync-trunk\runtimes\cpp\platforms\sdl\MoRE\

REM goto END

cd \mosync-trunk\runtimes\cpp\platforms\sdl\MoRE\
MoRE
cd c:\mosync-trunk\testPrograms\ruby\

:END
