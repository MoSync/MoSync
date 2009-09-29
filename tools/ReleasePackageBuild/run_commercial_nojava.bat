@echo PATH:
@echo %PATH%

set MOSYNCPOLISH_HAVE=-have-s60v3 -have-s60v2
set MOSYNCPOLISH_BUILD=-build-s60v3 -build-s60v2
call setwince.bat
call setcommercial.bat
set MOSYNC_INSTNAME_POSTFIX=_commercial_vm
call run.bat
