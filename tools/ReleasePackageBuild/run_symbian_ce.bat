@REM This file builds the public release package.

call setsymbian.bat
call setwince.bat
set MOSYNCPOLISH_HAVE=%MOSYNCPOLISH_HAVE% -arm-recompiler
call run.bat
