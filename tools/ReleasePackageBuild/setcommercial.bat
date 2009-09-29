call gensetcurrev.bat
call setcurrev.bat

set MOSYNCPOLISH_HAVE=%MOSYNCPOLISH_HAVE% -commercial -config config_convert_all.xml -version %MOSYNC_REVISION%
set COMMERCIAL=Y
