@echo off
set WTK2=c:\SonyEricsson\JavaME_SDK_CLDC\PC_Emulation\WTK2
set LIBDIR=%WTK2%\lib
set LIBCP=%LIBDIR%\cldcapi11.jar;%LIBDIR%\midpapi20.jar;%LIBDIR%\jsr082.jar
set TARGETDIR=%WTK2%\apps\MAVM\classes
set CLASSPATH=%LIBCP%;%TARGETDIR%

REM %WTK2%\bin\preverify.exe -d %TARGETCLASS% -classpath %CLASSPATH% %SRCCLASS%

@echo on

copy /Y StaticCode.class %WTK2%\apps\MAVM\tmpclasses
copy /Y resources %WTK2%\apps\MAVM\res
%WTK2%\bin\preverify.exe -d %TARGETDIR% -classpath %CLASSPATH% %WTK2%\apps\MAVM\tmpclasses
