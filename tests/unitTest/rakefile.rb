#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LSTFILES is a list of resource list files
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = [".", "Libs/MAStd/automated_tests", "Libs/MAUtil/automated_tests"]
EXTRA_INCLUDES = ["."]
EXTRA_CFLAGS = " -Wno-shadow"
PIPEFLAGS = " -datasize=1024000 -stacksize=128000 -heapsize=512000"

LOCAL_DLLS = ["mautil", "matest"]
EXENAME	= "unitTest"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/mosync.rb"
