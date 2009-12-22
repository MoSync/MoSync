@echo PATH:
@echo %PATH%

call setsymbian.bat
call setwince.bat
call setcommercial.bat
set MOSYNC_INSTNAME_POSTFIX=_commercial
set MOSYNC_RUNTIME_HAVE=%MOSYNC_RUNTIME_HAVE% -arm-recompiler
call run.bat
@IF NOT %ERRORLEVEL% == 0 exit /b 23
@exit /b 0
