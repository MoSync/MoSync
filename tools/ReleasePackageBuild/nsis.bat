@xcopy InstallerResources\*.* \mb\MoSyncReleasePackage /y
@IF NOT errorlevel 0 goto TOOL_ERROR
@xcopy build_package_tools\mosync_prerequisites\*.* \mb\MoSyncReleasePackage /y
@IF NOT errorlevel 0 goto TOOL_ERROR
@copy build_package_tools\mosync_docs\licenses\mosync-license.txt \mb\MoSyncReleasePackage\license.txt /y
@IF NOT errorlevel 0 goto TOOL_ERROR

build_package_tools\NSIS\makensis \mb\MoSyncReleasePackage\MoSync.nsi
@IF NOT errorlevel 0 goto TOOL_ERROR
