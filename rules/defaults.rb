require "rules/util.rb"

module Defaults
def set_defaults
	default(:CONFIG, "debug")

	default(:EXTRA_CFLAGS, "")
	default(:EXTRA_CPPFLAGS, "")
	default(:EXTRA_LINKFLAGS, "")
	default(:EXTRA_INCLUDES, [])
	default(:IGNORED_FILES, [])
	default(:EXTRA_SOURCEFILES, [])
	default(:EXTRA_OBJECTS, [])
	default(:LIBRARIES, [])
	default(:SPECIFIC_CFLAGS, {})
	default(:BUILDDIR_BASE, "build/")

	if(@CONFIG == "debug") then
		@BUILDDIR = @BUILDDIR_BASE + "debug/"
		@CONFIG_POSTFIX = "D"
	elsif(@CONFIG == "")
		@BUILDDIR = @BUILDDIR_BASE
		@CONFIG_POSTFIX = ""
	else
		error "wrong configuration: " + @CONFIG
	end
end
end
