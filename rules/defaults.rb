# Copyright (C) 2009 Mobile Sorcery AB
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

require "#{File.dirname(__FILE__)}/util.rb"
require "#{File.dirname(__FILE__)}/targets.rb"

# Defines default values for all variables that can be used by individual workfiles.
module Defaults
def set_defaults
	# Array of Strings, directories that will be searched for source files.
	default(:SOURCES, [])
	# String, extra flags used when compiling C files.
	default(:EXTRA_CFLAGS, "")
	# String, extra flags used when compiling C++ files.
	default(:EXTRA_CPPFLAGS, "")
	# String, extra flags used when linking applications with gcc or pipe-tool.
	default(:EXTRA_LINKFLAGS, "")
	# Array of Strings, extra include directories.
	default(:EXTRA_INCLUDES, [])
	# Array of Strings, names of files that should not be built.
	default(:IGNORED_FILES, [])
	# Array of Strings, paths to files that should be built, even though they are outside the SOURCES.
	default(:EXTRA_SOURCEFILES, [])
	# Array of FileTasks, precompiled object files, to link with.
	default(:EXTRA_OBJECTS, [])
	# Array of Strings, names of static libraries built earlier, to link with.
	default(:LOCAL_LIBS, [])
	# Array of Strings, names of shared libraries built earlier, to link with.
	default(:LOCAL_DLLS, [])
	# Array of Strings, names of Win32 MoSync precompiled libraries, to link with.
	default(:CUSTOM_LIBS, [])
	# Array of Strings, names of static libraries built earlier, to link with using GNU ld's --whole-archive option.
	default(:WHOLE_LIBS, [])
	# Array of Strings, names of libraries to link with.
	default(:LIBRARIES, [])
	# Hash(String,String). Key is the filename of a source file.
	# Value is extra compile flags to be used when compiling that file.
	default(:SPECIFIC_CFLAGS, {})
	# String, name of the base build directory.
	default(:BUILDDIR_BASE, "build/")
	
	# String, configuration identifier.
	# Valid values are "debug" and "" (optimized).
	default_const(:CONFIG, "debug")
	
	# @BUILDDIR is the name of the build directory, where generated files are stored.
	# @CONFIG_POSTFIX is a string appended to the filenames of some types of libraries,
	# to tell which configuration they were built with.
	if(CONFIG == "debug") then
		@BUILDDIR = @BUILDDIR_BASE + "debug/"
		@CONFIG_POSTFIX = "D"
	elsif(CONFIG == "")
		@BUILDDIR = @BUILDDIR_BASE
		@CONFIG_POSTFIX = ""
	else
		error "wrong configuration: " + CONFIG
	end
	
	default(:GCC_IS_V4, NATIVE_GCC_IS_V4)
	default(:GCC_IS_V43, NATIVE_GCC_IS_V43)
	
	# String, path to a common base directory for all workfiles in the project.
	default(:COMMON_BASEDIR, File.expand_path(File.dirname(__FILE__) + "/.."))
	# String, path to a common build directory.
	default(:COMMON_BUILDDIR, @COMMON_BASEDIR + "/" + @BUILDDIR)
	# String, path to a base directory which will be used as a target for executables and libraries.
	default(:TARGETDIR, @COMMON_BASEDIR)
end
end
