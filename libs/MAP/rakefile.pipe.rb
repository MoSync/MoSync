#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
IGNORED_FILES = ["MemoryMgr.cpp"]
SPECIFIC_CFLAGS = {"MapCache.cpp" => " -Wno-unreachable-code"}
LIBNAME = "map"
INSTALL_INCDIR = "MAP"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/pipe.rb"
