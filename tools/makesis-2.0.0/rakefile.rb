BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["src"]
EXTRA_INCLUDES = ["include"]
EXTRA_CFLAGS = " -Wno-shadow"
LIBRARIES = ["z"]

EXENAME = "makesis-200"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
