#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = [".", "../libsupc++"]
EXTRA_OBJECTS = ["crtlib.s"]
DEFAULT_LIBS = []
IGNORED_FILES = ["new_handler.cc", "MemoryPool.c"]
INSTALL_INCDIR = "."
IGNORED_HEADERS = ["math_private.h", "fdlibm.h"]
#EXTRA_CFLAGS = " -Wno-strict-aliasing"
SPECIFIC_CFLAGS = {
	"maint64.c" => " -Wno-missing-declarations",
	"madmath.c" => " -Wno-missing-declarations",
	"mavsprintf.c" => " -Wno-float-equal",
	"strtod.c" => " -Wno-float-equal",
	"e_log.c" => " -Wno-float-equal",
	"s_atan.c" => " -Wno-strict-aliasing",
	"e_atan2.c" => " -Wno-strict-aliasing",
	"e_asin.c" => " -Wno-strict-aliasing",
	"maint64.c" => " -Wno-strict-aliasing"
	}

LIBNAME = "mastd"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/pipe.rb"
