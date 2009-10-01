@REM This file builds the public release package.

call setsymbian.bat
call setwince.bat
set MOSYNC_HAVE=%MOSYNC_HAVE% -arm-recompiler
call run.bat
