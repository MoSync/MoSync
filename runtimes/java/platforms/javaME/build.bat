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

call process ../../shared Core 1
call process src Syscall 2
call process src MAMidlet 3
call process src MainCanvas 4
call process ../../shared BigPhatError 5
call process ../../shared Tilemap 6
call process ../../shared Tileset 7
call process ../../shared LittleEndianDataInputStream 8
call process ../../shared LimitedLengthInputStream 9
call process ../../shared BinaryInterface 10
call process ../../shared Binary 11
call process ../../shared UBin 12
call process ../../shared RefByteArrayOutputStream 13
call process ../../shared ThreadPool 14
call process ../../shared GdbStub 15
call process src PngStream 16
call process ../../shared ImageCache 17
call process ../../shared BufferlessPipeStream 18
call process ../../shared LoggingInputStream 19
call process ../../shared ImageCache 20
call process ../../shared BufferlessPipeStream 21
call process src MoSync 22
call process src ExtensionHandler 23
call process ../../shared MAPanicReport 24
@REM call process src AudioBufferInputStream
@REM call process src Pcm2WavInputStream
call process src AudioBufferDataSource 25

@REM temporary
call process src Real 26

:END
