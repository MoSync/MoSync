#---------------------------------------------------------------------------------
# MoSync Base Directory
#---------------------------------------------------------------------------------

BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
EXTRA_SOURCEFILES = ["../kazlib/dict.c", "../kazlib/hash.c"]
IGNORED_FILES = ["DomParser.cpp", "XML.cpp", "XML_stack.cpp", "XMLDataProvider.cpp",
	"XPathTokenizer.cpp", "Stream_utf8.cpp", "Stream_conn.cpp"]
EXTRA_CXXFLAGS = " -fno-exceptions"

DLLNAME = "mautil"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native_mosync.rb"
