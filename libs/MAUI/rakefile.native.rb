#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
EXTRA_CFLAGS = " -Wno-shadow"	#TODO, low-prio: enable -Wshadow
IGNORED_FILES = ["ScrollPane.cpp"]

LOCAL_DLLS = ["mautil"]
DLLNAME = "maui"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native_mosync.rb"
