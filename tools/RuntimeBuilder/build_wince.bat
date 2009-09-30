@echo on
call "%VS80COMNTOOLS%/vsvars32.bat"
set OLDDIR=%CD%
cd %1
vcbuild MoRE-winmobile.vcproj %2
cd %OLDDIR%
