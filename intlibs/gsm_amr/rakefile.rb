BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
EXTRA_INCLUDES = ["amr_nb/common/include","oscl","amr_nb/dec/include","common/dec/include","amr_wb/dec/include"]
SOURCES = ["amr_nb/common/src","amr_nb/dec/src","common/dec/src","amr_wb/common/src","amr_wb/dec/src"]
DLLNAME = "amr"
EXTRA_CFLAGS = " -DC_EQUIVALENT -Wno-undef"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native"
