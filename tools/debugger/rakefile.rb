BD = "../.."

# rule for code generation
task generate => ["operationsGen.rb", "../../intlibs/stabs/types.rb"] do |t|
	sh "ruby operationsGen.rb"
end

file "operations_generated.h" => generate
file "operations_generated.cpp" => [generate, "operations_generated.h"]

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------

SOURCES = ["."]
EXTRA_SOURCEFILES = ["#{BD}/intlibs/helpers/intutil.cpp", "../pipe-tool/disas.c",
	"#{BD}/intlibs/helpers/TranslateSyscall.cpp", "operations_generated.cpp"]
EXTRA_CFLAGS = " -DPTAH_EXPORTS -DLOGGING_ENABLED"
SPECIFIC_CFLAGS = { "expression.cpp" => " -Wno-shadow" }
EXTRA_INCLUDES = ["#{BD}/intlibs", "#{BD}/runtimes/java/source", "#{BD}/runtimes/cpp/core", "../pipe-tool"]
LOCAL_LIBS = ["mosync_log_file", "mosync_bluetooth", "net", "stabs"]
LIBRARIES = ["SDL"]

EXENAME = "mdb"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
