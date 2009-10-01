@echo PATH:
@echo %PATH%

call setsymbian.bat
call setwince.bat
call setcommercial.bat
set MOSYNC_INSTNAME_POSTFIX=_commercial
set MOSYNC_HAVE=%MOSYNC_HAVE% -arm-recompiler
call run.bat
