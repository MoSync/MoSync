# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require "#{File.dirname(__FILE__)}/native_gcc.rb"
require "#{File.dirname(__FILE__)}/flags.rb"

# Base class.
# Links object files together to form an native executable or shared library.
class NativeGccLinkTask < FileTask
	def initialize(work, name, objects, linker)
		super(work, name)
		initFlags
		@prerequisites += @objects = objects
		@linker = linker
	end

	def needed?(log = true)
		return true if(super(log))
		return if(flagsNeeded?(log))
	end

	def cFlags
		return "\"#{@objects.join('" "')}\"#{@FLAGS}"
	end

	def execute
		execFlags
		#puts "objects: #{@objects.join(' ')}"
		#puts "flags: #{@FLAGS}"
		sh "#{@linker} #{cFlags} -o \"#{@NAME}\""
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

	def setup3(all_objects, have_cppfiles)
		if(HOST == :darwin)
			@EXTRA_LINKFLAGS += " -m32 -L/sw/lib -L/opt/local/lib -framework Cocoa -framework IOBluetooth -framework Foundation"
		end
		if(PROFILING)
			@EXTRA_LINKFLAGS += " -pg"
		end
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		lld = @LOCAL_DLLS.collect { |ld| FileTask.new(self, @COMMON_BUILDDIR + ld + DLL_FILE_ENDING) }
		wlo = @WHOLE_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		# This comment fixes a build problem. Don't ask me why!
		@LIBRARIES.each { |l| @EXTRA_LINKFLAGS += " -l" + l }
		need(:@NAME)
		need(:@BUILDDIR)
		need(:@TARGETDIR)
		target = @TARGETDIR + "/" + @BUILDDIR + @NAME + link_file_ending
		#puts "@EXTRA_LINKFLAGS: "+@EXTRA_LINKFLAGS
		linker = have_cppfiles ? 'g++' : 'gcc'
		@TARGET = link_task_class.new(self, target, all_objects, wlo, llo + lld, @EXTRA_LINKFLAGS, linker)
		@prerequisites += [@TARGET]
	end
end
