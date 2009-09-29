BD = "../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
LOCAL_LIBS = ["idl-common", "filelist"]
if(HOST == "win32") then
	EXETARGET = "build/idl2.exe"
else
	EXETARGET = "build/idl2"
end

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"

directory "#{MOSYNCDIR}/bin/"

# let's pick a late target, to minimize the chances of something going wrong.
# todo: remove target file in case of fail, to ensure success later.
TARGETFILE = "#{BD}/runtimes/cpp/core/invoke_syscall_cpp.h"

task :compile => [TARGETFILE, "#{MOSYNCDIR}/bin/"]
	
file TARGETFILE => [:default, "extensions.h"] + FileList["*.idl"] do
	sh EXETARGET
end

task :clean do
	sh "rm -rf Output"
	#TODO: remove all copied files
end
