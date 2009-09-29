@echo ------------------------------------------------
@echo Building docbook.
@echo ------------------------------------------------

cd %MOSYNC_TRUNK%\docs\docbook
call makeHtml.bat
mkdir %MOSYNC_PATH%\docs\gettingstarted
copy output\guides\*.html %MOSYNC_PATH%\docs\gettingstarted\
copy *.css %MOSYNC_PATH%\docs\gettingstarted\
@REM copy %MOSYNC_TRUNK%\docs\gettingstarted\*.* %MOSYNC_PATH%\docs\gettingstarted\

xcopy gfx %MOSYNC_PATH%\docs\gettingstarted\gfx\ /E /Y /D
xcopy images %MOSYNC_PATH%\docs\gettingstarted\images\ /E /Y /D
del /Q %MOSYNC_PATH%\docs\gettingstarted\images\*.psd

cd %ORIGINAL_PATH%

@echo.
