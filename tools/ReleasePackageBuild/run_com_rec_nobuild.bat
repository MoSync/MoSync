@echo PATH:
@echo %PATH%

call setsymbian.bat
call setwince.bat
call setcommercial.bat
set MOSYNC_INSTNAME_POSTFIX=_commercial
set MOSYNC_RUNTIME_HAVE=%MOSYNC_RUNTIME_HAVE% -arm-recompiler
set MOSYNC_RUNTIME_BUILD= 
call run.bat
