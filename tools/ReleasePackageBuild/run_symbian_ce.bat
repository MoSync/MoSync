@REM This file builds the public release package.

call setsymbian.bat
call setwince.bat
set MOSYNC_RUNTIME_HAVE=%MOSYNC_RUNTIME_HAVE% -arm-recompiler
call run.bat
