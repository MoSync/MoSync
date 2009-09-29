BD = "../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
if(HOST == "win32") then
	CUSTOM_LIBS = ["freeimage.lib"]
else
	LIBRARIES = ["freeimage"]
end

EXENAME = "FontGenerator"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
