@echo off

IF EXIST setdirs.bat GOTO STD
@echo Error: setdirs.bat missing
GOTO END
:STD

call setdirs.bat

echo Clearing directories...

call cleardir %TARGETDIR%\src
call cleardir %TARGETDIR%\classes
call cleardir %TARGETDIR%\tmpclasses

call cleardir build

echo Preprocessing...
@REM echo on

call process ../../shared BigPhatError 1
call process ../../shared Binary 2
call process ../../shared BinaryInterface 3
call process ../../shared Core 4
call process ../../shared LimitedLengthInputStream 5
call process ../../shared LittleEndianDataInputStream 6
call process ../../shared ThreadPool 7
call process ../../shared UBin 8
call process ../../shared RefByteArrayOutputStream 9
call process ../../shared ImageCache 12
call process ../../shared MAPanicReport 13

call process src Syscall 14
call process src MAMidlet 15
call process src MainCanvas 16
call process src PngStream 17
call process src MoSync 18
call process src ExtensionHandler 19
call process src AudioBufferDataSource 20
call process src Real 21
@REM call process src AudioBufferInputStream 22
@REM call process src Pcm2WavInputStream 23

:END
