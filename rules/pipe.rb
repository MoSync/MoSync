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

require "#{File.dirname(__FILE__)}/gcc.rb"
#require 'lib/importenv.rb'

module MoSyncInclude
	def mosync_include; "#{ENV['MOSYNCDIR']}/include" + sub_include; end
	def mosync_libdir; "#{ENV['MOSYNCDIR']}/lib"; end
	def sub_include; USE_NEWLIB ? "/newlib" : ""; end
end

class PipeTask < FileTask
	def initialize(work, name, objects, linkflags)
		super(work, name)
		@FLAGS = linkflags
		dirTask = DirTask.new(work, File.dirname(name))
		@objects = objects
		@prerequisites = @objects + [dirTask]
	end
	def execute
		# pipe-tool may output an empty file and then fail.
		begin
			sh "#{ENV["MOSYNCDIR"]}/bin/pipe-tool#{@FLAGS} #{@NAME} #{@objects.join(' ')}"
		rescue => e
			FileUtils.rm_f(@NAME)
			raise
		end
		if(!File.exist?(@NAME))
			error "Pipe-tool failed silently!"
		end
	end
end

# adds dependency handling
class PipeResourceTask < PipeTask
	def initialize(work, name, objects)
		@depFile = "#{File.dirname(name)}/resources.mf"
		@tempDepFile = "#{@depFile}t"
		super(work, name, objects, " -depend=#{@tempDepFile} -R")
		
		# only if the file is not already needed do we care about extra dependencies
		if(!needed?(false)) then
			@prerequisites = MakeDependLoader.load(@depFile, @NAME)
		end
	end
	def needed?(log = true)
		if(!File.exists?(@depFile))
			puts "Because the dependency file is missing:" if(log)
			return true
		end
		return super(log)
	end
	def execute
		super
		FileUtils.mv(@tempDepFile, @depFile)
	end
end

class PipeGccWork < GccWork
	def gccVersionClass; PipeGccWork; end
	include GccVersion
	def gcc; ENV["MOSYNCDIR"] + "/bin/xgcc"; end
	def gccmode; "-S"; end
	def host_flags; USE_NEWLIB ? " -DUSE_NEWLIB" : ""; end
	def host_cppflags; ""; end
	
	include MoSyncInclude
	
	def set_defaults
		@BUILDDIR_BASE = "build/pipe/"
		super
	end
	
	private
	
	def object_ending; ".s"; end
	
	def setup3(all_objects)
		#puts all_objects
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".lib") }
		need(:@NAME)
		if(@TARGET_PATH == nil)
			need(:@BUILDDIR)
			need(:@TARGETDIR)
			@TARGET_PATH = @TARGETDIR + "/" + @BUILDDIR + "program"
		end
		@TARGET = PipeTask.new(self, @TARGET_PATH, all_objects + llo, @FLAGS + @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
