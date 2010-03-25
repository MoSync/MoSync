BD = "../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = []
if(HOST == "win32") then
	EXTRA_SOURCEFILES = ["filelist-win32.c"]
else
	EXTRA_SOURCEFILES = ["filelist-linux.c"]
end
LIBNAME = "filelist"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
