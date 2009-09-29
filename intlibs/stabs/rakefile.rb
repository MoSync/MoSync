BD = "../.."

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
EXTRA_SOURCEFILES = ["#{BD}/runtimes/cpp/core/sld.cpp"]
EXTRA_INCLUDES = ["#{BD}/runtimes/cpp/core"]
LIBNAME = "stabs"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native"

# rule for code generation
file "stabs_builtins_generated.h" => ["typeGen.rb", "types.rb"] do |t|
	sh "ruby typeGen.rb"
end

# make all objects depend on the generated file, so its task gets executed first.
SOURCE_OBJECTS.each do |o| task o => "stabs_builtins_generated.h" end
