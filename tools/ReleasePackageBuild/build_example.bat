@REM Designed to be called from build_package.bat
@REM %2 is example name ("3dLines", "MoSketch)

@set MA_ERRORLEVEL=0

cd %MOSYNC_PATH%\examples\%1\%2
mobuild -pack-all %2.mopro
@IF NOT errorlevel 0 goto TOOL_ERROR
cd %ORIGINAL_PATH%

@goto BE_END

:TOOL_ERROR
@set MA_ERRORLEVEL=1

:BE_END
