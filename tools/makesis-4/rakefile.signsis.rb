BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["src"]
EXTRA_INCLUDES = ["include", "src"]
IGNORED_FILES = ["makekeys.cpp", "makesis.cpp", "dumptree.cpp", "dumpcontroller.cpp", "finddatetime.cpp"]
EXTRA_CFLAGS = " -Wno-shadow"
LIBRARIES = ["z"]
CUSTOM_LIBS = ["libeay32.lib"]

EXENAME = "signsis-4"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
