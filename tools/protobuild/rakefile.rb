BD = "../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
#EXTRA_CFLAGS = " -Wno-write-strings -Wno-unused-function -Wno-multichar -D_CRT_SECURE_NO_DEPRECATE"
#LIBRARIES = ["freeimage"]
EXENAME = "protobuild"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
