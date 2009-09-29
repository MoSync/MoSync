#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LSTFILES is a list of resource list files
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
EXTRA_CFLAGS = " -Wno-float-equal"

LOCAL_DLLS = ["mautil"]
EXENAME	= "timers"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/mosync.rb"
