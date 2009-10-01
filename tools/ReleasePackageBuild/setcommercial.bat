call gensetcurrev.bat
call setcurrev.bat

set MOSYNC_HAVE=%MOSYNC_HAVE% -commercial -config config_convert_all.xml -version %MOSYNC_REVISION%
set COMMERCIAL=Y
