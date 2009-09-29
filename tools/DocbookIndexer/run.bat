call setjavacpath.bat
set BASEDIR=%CD%
cd src\com\mobilesorcery\mosync\docbook\
call buildrun.bat %BASEDIR%\..\ReleasePackageBuild\build_package_tools\bin\docbookParser-lib %1
