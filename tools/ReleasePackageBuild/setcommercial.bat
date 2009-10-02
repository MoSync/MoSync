call gensetcurrev.bat
call setcurrev.bat

set MOSYNC_RUNTIME_HAVE=%MOSYNC_RUNTIME_HAVE% -commercial -config config_convert_all.xml -version %MOSYNC_REVISION%
set COMMERCIAL=Y
