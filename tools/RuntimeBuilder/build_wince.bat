@echo on
call "%VS80COMNTOOLS%/vsvars32.bat"
set OLDDIR=%CD%
cd %1
vcbuild MoRE-winmobile.vcproj %2
@IF NOT %ERRORLEVEL% == 0 goto TOOL_ERROR

@goto END

:TOOL_ERROR
@echo Error building the runtime.
exit /b 1

:END
cd %OLDDIR%
exit /b 0

