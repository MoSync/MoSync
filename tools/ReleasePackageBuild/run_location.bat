call gensetcurrev.bat
call setcurrev.bat

set MOSYNCPOLISH_HAVE=-commercial -config config_convert_location_api.xml -version %MOSYNC_REVISION%
set COMMERCIAL=Y

call run.bat
