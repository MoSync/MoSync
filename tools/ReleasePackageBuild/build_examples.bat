@REM Designed to be called from build_package.bat

@call build_example . debugging
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR

@call build_example Moblet HelloMoblet
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example Moblet simple
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example Moblet timers
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example Moblet Stylus
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example Moblet Connection
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR

@REM		call build_example MAUI clock
@REM		IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@REM		call build_example MAUI multi
@REM		IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example MAUI MAUIex
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR


@call build_example . 3dLines
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . AdvGraphics
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . MoSketch
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . MAStx
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . OtaLoad
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@REM           call build_example . MAXml
@REM           IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . MoTooth
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . MoSound
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . MoTrix
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . Tiles
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR
@call build_example . MoTris
@IF NOT %MA_ERRORLEVEL%==0 goto TOOL_ERROR

:TOOL_ERROR
