@echo off
set WTK2=c:\SonyEricsson\JavaME_SDK_CLDC\PC_Emulation\WTK2
set LIBDIR=%WTK2%\lib
set LIBCP=%LIBDIR%\cldcapi11.jar;%LIBDIR%\midpapi20.jar;%LIBDIR%\jsr082.jar;%LIBDIR%\jsr179.jar;%LIBDIR%\wma20.jar
set BASEDIR=%WTK2%\apps\JavaVM
set TARGETDIR=%BASEDIR%\classes
@REM set CLASSPATH=%LIBCP%;%TARGETDIR%
set CLASSPATH="C:\Program\Research In Motion\BlackBerry JDE 5.0.0\lib\net_rim_api.jar"

REM %WTK2%\bin\preverify.exe -d %TARGETCLASS% -classpath %CLASSPATH% %SRCCLASS%

@echo on

copy /Y StaticCode.class %BASEDIR%\tmpclasses
copy /Y StaticCode.class c:\code\blackberry\
copy /Y resources %BASEDIR%\res
%WTK2%\bin\preverify.exe -d %TARGETDIR% -classpath %CLASSPATH% %BASEDIR%\tmpclasses
