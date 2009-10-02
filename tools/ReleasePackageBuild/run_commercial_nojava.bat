@echo PATH:
@echo %PATH%

set MOSYNC_RUNTIME_HAVE=-have-s60v3 -have-s60v2
set MOSYNC_RUNTIME_BUILD=-build-s60v3 -build-s60v2
call setwince.bat
call setcommercial.bat
set MOSYNC_INSTNAME_POSTFIX=_commercial_vm
call run.bat
