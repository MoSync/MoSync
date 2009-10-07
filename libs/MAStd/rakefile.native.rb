#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
EXTRA_INCLUDES = ["."]
IGNORED_FILES = ["e_acos.c", "e_asin.c", "e_atan2.c", "e_pow.c", "matask.c", "modf.c",
	"s_atan.c", "s_ceil.c", "s_floor.c", "s_frexp.c", "maint64.c", "e_log.c",
	"strtod.c", "macpp.cpp", "MemoryPool.c"]
#EXTRA_CFLAGS = " -Wno-strict-aliasing"
SPECIFIC_CFLAGS = {
#	"maint64.c" => " -Wno-missing-declarations",
	"madmath.c" => " -Wno-missing-declarations",
	"mavsprintf.c" => " -Wno-float-equal"}
#	"strtod.c" => " -Wno-float-equal",
#	"e_log.c" => " -Wno-float-equal"}

LOCAL_DLLS = ["mosync"]
DLLNAME = "mastd"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
