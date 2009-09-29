@echo PATH:
@echo %PATH%

call setsymbian.bat
call setwince.bat
call setcommercial.bat
set MOSYNC_INSTNAME_POSTFIX=_commercial
set MOSYNCPOLISH_HAVE=%MOSYNCPOLISH_HAVE% -arm-recompiler
call run.bat
