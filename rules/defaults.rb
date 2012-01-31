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

# load local_config.rb, if it exists.
lc = "#{File.dirname(__FILE__)}/local_config.rb"
require lc if(File.exists?(lc))

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
	# Array of Strings, names of files that should not be compiled.
	default(:IGNORED_FILES, [])
	# Array of Strings, paths to files that should be compiled, even though they are outside the SOURCES.
	default(:EXTRA_SOURCEFILES, [])
	# Array of FileTasks, generated source files that should be compiled along with the others.
	default(:EXTRA_SOURCETASKS, [])
	# Array of FileTasks, precompiled object files, to link with.
	default(:EXTRA_OBJECTS, [])

	# Array of Tasks that should be invoked before the others.
	# For example, resource generation.
	default(:PREREQUISITES, [])

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

	# Array of Strings, paths to LST files containing resource definitions.
	# If this is false or nil, it will be populated from @SOURCES.
	default(:LSTFILES, false)
	# String, path to file to be fed into rescomp.
	# Overrides @LSTFILES and @resourceTask.
	default(:LSTX, false)
	# String, platforms used by rescomp.
	default(:RES_PLATFORM, '')

	# Hash(String,String). Key is the filename of a source file.
	# Value is extra compile flags to be used when compiling that file.
	default(:SPECIFIC_CFLAGS, {})

	# String, extra flags used when invoking MoRE in PipeExeWork's automatic "run" target
	default(:EXTRA_EMUFLAGS, '')

	# String, name of the base build directory.
	default(:BUILDDIR_BASE, "build/")
	# String, added to the beginning of build directories.
	default(:BUILDDIR_PREFIX, "")
	# String, added to the beginning of the common build directories.
	default(:COMMOM_BUILDDDIR_PREFIX, "")

	# String, configuration identifier.
	# Valid values are "debug" and "" (optimized).
	default_const(:CONFIG, "debug")

	# If true, the GCC version number will be added to BUILDDIR names for native projects.
	# Useful if you often switch between different versions of GCC.
	default_const(:USE_GCC_VERSION_IN_BUILDDIR_NAME, false)
	if(USE_GCC_VERSION_IN_BUILDDIR_NAME)
		builddir_postfix = '_'+@gcc_version.strip
	else
		builddir_postfix = ''
	end

	# @BUILDDIR is the name of the build directory, where generated files are stored.
	# @CONFIG_NAME is the name of the configuration.
	if(CONFIG == "debug") then
		@CONFIG_NAME = "debug"
	elsif(CONFIG == "")
		@CONFIG_NAME = "release"
	else
		error "wrong configuration: " + CONFIG
	end
	default(:BUILDDIR_NAME, @BUILDDIR_PREFIX + @CONFIG_NAME + builddir_postfix)
	default(:BUILDDIR, @BUILDDIR_BASE + @BUILDDIR_NAME + "/")

	default(:COMMON_BUILDDIR_NAME, @COMMOM_BUILDDDIR_PREFIX + @CONFIG_NAME + builddir_postfix)

	# String, path to a common base directory for all workfiles in the project.
	default(:COMMON_BASEDIR, File.expand_path_fix(File.dirname(__FILE__) + "/.."))
	# String, path to a common build directory.
	default(:COMMON_BUILDDIR, @COMMON_BASEDIR + "/" + @BUILDDIR_BASE + @COMMON_BUILDDIR_NAME + "/")
	# String, path to a base directory which will be used as a target for executables and libraries.
	default(:TARGETDIR, @COMMON_BASEDIR)

	# String, path to a directory. If set, the Work's target will be copied there.
	default(:INSTALLDIR, nil)

	# Array of Arrays of Strings, list of extensions to use.
	# Each sub-array must contain two strings: [path to idl file, name of extension].
	# Only used by PipeExeWork.
	default(:EXTENSIONS, nil)
end
end
