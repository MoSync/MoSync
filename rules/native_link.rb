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

require "#{File.dirname(__FILE__)}/native_gcc.rb"
require "#{File.dirname(__FILE__)}/flags.rb"

# Base class.
# Links object files together to form an native executable or shared library.
class NativeGccLinkTask < FileTask
	def initialize(work, name, objects)
		super(work, name)
		initFlags
		@prerequisites = @objects = objects
	end
	
	def needed?(log = true)
		return true if(flagsNeeded?(log))
		super(log)
	end
	
	def execute
		execFlags
		#puts "objects: #{@objects.join(' ')}"
		#puts "flags: #{@FLAGS}"
		sh "g++ #{@objects.join(' ')}#{@FLAGS} -o #{@NAME}"
	end
	
	include FlagsChanged
end

# Base class.
# Compiles C/C++ code into an native executable file or shared library.
# Supports GCC on mingw and linux.
# In addition to the variables used by GccWork, this class uses the following:
# @NAME, @LOCAL_LIBS, @LOCAL_DLLS, @WHOLE_LIBS, @LIBRARIES, @COMMON_BUILDDIR, @BUILDDIR and @TARGETDIR.
class NativeGccLinkWork < NativeGccWork
	private

	def setup3(all_objects)
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		lld = @LOCAL_DLLS.collect { |ld| FileTask.new(self, @COMMON_BUILDDIR + ld + DLL_FILE_ENDING) }
		wlo = @WHOLE_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		@LIBRARIES.each { |l| @EXTRA_LINKFLAGS += " -l" + l }
		need(:@NAME)
		need(:@BUILDDIR)
		need(:@TARGETDIR)
		target = @TARGETDIR + "/" + @BUILDDIR + @NAME + link_file_ending
		#puts "@EXTRA_LINKFLAGS: "+@EXTRA_LINKFLAGS
		@TARGET = link_task_class.new(self, target, all_objects, wlo, llo + lld, @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
