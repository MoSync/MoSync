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

require "#{File.dirname(__FILE__)}/host.rb"
require "#{File.dirname(__FILE__)}/task.rb"
require "#{File.dirname(__FILE__)}/gcc_flags.rb"
require "#{File.dirname(__FILE__)}/loader_md.rb"
require "#{File.dirname(__FILE__)}/flags.rb"

def get_gcc_version_string(gcc)
	file = open("|#{gcc} -v 2>&1")
	file.each do |line|
		parts = line.split(/ /)
		#puts "yo: #{parts.inspect}"
		if(parts[0] == "gcc" && parts[1] == "version")
			return parts[2]
		end
	end
	error("Could not find gcc version.")
end

#warning("GCC version: #{GCC_VERSION}")
#warning("GCC_IS_V4: #{GCC_IS_V4}")
#warning("GCC_IS_V43: #{GCC_IS_V43}")

# Compiles a source file using gcc.
# Generates extra files for tracking dependencies and flags,
# so that if the flags or any dependency have changed, this file will be recompiled.
# Objects of this class are created by GccWork.
class CompileGccTask < FileTask
	def initialize(work, name, source, cflags)
		super(work, name)
		@SOURCE = source
		@prerequisites = [source]
		
		@DEPFILE = @work.genfile(source, ".mf")
		@TEMPDEPFILE = @work.genfile(source, ".mft")
		depFlags = " -MMD -MF #{@TEMPDEPFILE}"
		@FLAGS = cflags + depFlags
		
		initFlags
		
		# only if the file is not already needed do we care about extra dependencies
		if(!needed?(false)) then
			@prerequisites = MakeDependLoader.load(@DEPFILE, @NAME)
		end
	end
	
	def needed?(log = true)
		return true if(super(log))
		if(!File.exists?(@DEPFILE))
			puts "Because the dependency file is missing:" if(log)
			return true
		end
		return flagsNeeded?(log)
	end
	
	def execute
		execFlags
		sh "#{@work.gcc} -o #{@NAME}#{@FLAGS} #{@work.gccmode} #{@SOURCE}"
		
		# In certain rare cases (error during preprocess caused by a header file)
		# gcc may output an empty dependency file, resulting in an empty dependency list for
		# the object file, which means it will not be recompiled, even though it should be.
		FileUtils.mv(@TEMPDEPFILE, @DEPFILE)
	end
	
	include FlagsChanged
end

# Base class.
# Compiles C/C++ code into an executable file.
# Supports GCC on mingw, pipe and linux.
# Uses the following variables: @SOURCES, @IGNORED_FILES, @EXTRA_SOURCEFILES,
# @SPECIFIC_CFLAGS and @EXTRA_OBJECTS.
# Requires subclasses to implement methods 'gcc', 'gccmode' and 'object_ending'.
class GccWork < BuildWork
	# Returns a path representing a generated file, given a source filename and a new file ending.
	def genfile(source, ending)
		@BUILDDIR + File.basename(source.to_s).ext(ending)
	end
	
	# The filename of the target.
	def target
		@TARGET
	end
	
	def set_defaults
		if(!defined?(@@GCC_IS_V4))
			gcc_version = get_gcc_version_string(gcc)
			@@GCC_IS_V4 = (gcc_version[0] == "4"[0])
			@@GCC_IS_V43 = (@@GCC_IS_V4 && (gcc_version[2] == "3"[0]))
			@@GCC_IS_V44 = (@@GCC_IS_V4 && (gcc_version[2] == "4"[0]))
		end
		@GCC_IS_V4 = @@GCC_IS_V4
		@GCC_IS_V43 = @@GCC_IS_V43
		@GCC_IS_V44 = @@GCC_IS_V44
		super
	end
	
	private
	
	include GccFlags
	
	def setup2
		define_cflags
		@CFLAGS_MAP = { ".c" => @CFLAGS,
			".cpp" => @CPPFLAGS,
			".cc" => @CPPFLAGS }
		
		#find source files
		cfiles = collect_files(".c")
		cppfiles = collect_files(".cpp") + collect_files(".cc") 
		@all_sourcefiles = cfiles + cppfiles
		
		@source_objects = objects(@all_sourcefiles)
		all_objects = @source_objects + @EXTRA_OBJECTS
		
		setup3(all_objects)
 	end
	
	# returns an array of FileTasks
	def collect_files(ending)
		files = @SOURCES.collect {|dir| Dir[dir+"/*"+ending]}
		files.flatten!
		files.reject! {|file| @IGNORED_FILES.member?(File.basename(file))}
		files += @EXTRA_SOURCEFILES.select do |file| file.getExt == ending end
		return files.collect do |file| FileTask.new(self, file) end
	end
	
	def getGccFlags(source)
		ext = source.to_s.getExt
		cflags = @CFLAGS_MAP[ext]
		if(cflags == nil) then
			error "Bad ext: '#{ext}' from source '#{source}'"
		end
		need(:@SPECIFIC_CFLAGS)
		cflags += @SPECIFIC_CFLAGS.fetch(File.basename(source.to_s), "")
		return cflags
	end
	
	def makeGccTask(source, ending)
		objName = genfile(source, ending)
		task = CompileGccTask.new(self, objName, source, getGccFlags(source))
		return task
	end
	
	# returns an array of CompileGccTasks
	def objects(sources)
		return sources.collect do |path| makeGccTask(path, object_ending) end
	end
end
