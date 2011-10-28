#!/usr/bin/ruby
# The above line tells BASH and similar shells to use the Ruby interpreter
# to execute this file.
# On Windows, you can associate .rb files with ruby.exe to achieve the same effect.

# First, load the rules needed to build a MoSync program.
require File.expand_path ENV['MOSYNCDIR']+'/rules/mosync_exe.rb'

# Create a Work object.
work = PipeExeWork.new
# Set the object's variables.
# There are many variables that will be recognized by the system.
# Only two are required for every Work. The rest are optional.
work.instance_eval do
	# The name of the program.
	@NAME = 'My app'
	# Array of Strings, directories that will be searched for source files.
	@SOURCES = ['src', 'src/subsystem']

	# Optional variables:

	# Array of Strings, extra include directories.
	@EXTRA_INCLUDES = ['src', 'src/includes']

	# String, extra flags used when compiling C files.
	# See the GCC documentation for details.
	@EXTRA_CFLAGS = ' -pedantic -ansi'

	# String, extra flags used when compiling C++ files.
	@EXTRA_CPPFLAGS = ' -fno-gnu-keywords'

	# Hash(String,String). Key is the filename of a source file.
	# Value is extra flags to be used when compiling that file.
	@SPECIFIC_CFLAGS = {
		'a.cpp' => ' -Wno-float-equal',
		'b.cpp' => ' -DPREDEIFINED_MACRO=1',
	}

	# String, extra flags used when linking applications with pipe-tool.
	@EXTRA_LINKFLAGS = ' -xerr' + standardMemorySettings(11)

	# Array of Strings, names of files that should not be compiled.
	@IGNORED_FILES = ['broken_file.cpp']

	# Array of Strings, paths to files that should be compiled,
	# even though they are outside @SOURCES.
	@EXTRA_SOURCEFILES = ['foo/bar.cpp']

	# Array of FileTasks, generated source files that should be compiled along with the others.
	mgft = MemoryGeneratedFileTask.new(self, 'build/alphabet.c')
	mgft.instance_eval do
		@buf = "const char* gAlphabet = \""
		('a'..'z').each |letter|
			@buf += letter
		end
		@buf += "\";\n"
	end
	@EXTRA_SOURCETASKS = [mgft]

	# Array of FileTasks, precompiled object files, to link with.
	@EXTRA_OBJECTS = [
		FileTask.new(self, 'src/mosync_assembly_code.s'),
	]

	# Array of Strings, names of static libraries built earlier, to link with.
	@LOCAL_LIBS = ['my_lib']

	# Array of Strings, names of libraries to link with.
	@LIBRARIES = ['mautil', 'mtxml']

	# String, extra flags used when invoking MoRE in PipeExeWork's automatic "run" target.
	@EXTRA_EMUFLAGS = ' -allowdivzero'

	# String, extra flags for the tool 'package'.
	@PACK_PARAMETERS = ' --s60v3uid e1234567 --s60v2uid 01234567'
end

# Finally, invoke the work.
# This will build all files that have not yet been built,
# or have had their prerequisites changed.
work.invoke

# Examples of invoking workfiles like this from the command line:

# Debug build:
# ruby workfile.rb

# Release build:
# ruby workfile.rb CONFIG=

# Run in emulator:
# ruby workfile.rb run

# Create installable package:
# ruby workfile.rb PACK="Samsung/Galaxy S"
