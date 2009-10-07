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
EXTRA_CFLAGS = " -Wno-shadow"

LOCAL_DLLS = ["mautil", "maui"]
PIPEFLAGS = " -datasize=256000 -heapsize=128000 -stacksize=16000"
EXENAME	= "mauiex"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/mosync.rb"
