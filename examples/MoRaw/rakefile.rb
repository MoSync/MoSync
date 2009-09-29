#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LSTFILES is a list of resource list files
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]

LOCAL_DLLS = [ "mafs", "mautil" ]
EXENAME	= "moraw"

PIPEFLAGS = " -datasize=4096000 -heapsize=2048000 -xerr -master-dump "

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/mosync.rb"
