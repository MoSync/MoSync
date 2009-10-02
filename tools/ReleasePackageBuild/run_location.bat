call gensetcurrev.bat
call setcurrev.bat

set MOSYNC_RUNTIME_HAVE=-commercial -config config_convert_location_api.xml -version %MOSYNC_REVISION%
set COMMERCIAL=Y

call run.bat
