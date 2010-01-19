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

class PipeTask < FileTask
	def initialize(work, name, objects, linkflags)
		super(work, name)
		@FLAGS = linkflags
		dirTask = DirTask.new(work, File.dirname(name))
		@objects = objects
		@prerequisites = @objects + [dirTask]
	end
	def execute
		sh "#{ENV["MOSYNCDIR"]}/bin/pipe-tool#{@FLAGS} #{@NAME} #{@objects.join(' ')}"
		if(!File.exist?(@NAME))
			error "Pipe-tool failed silently!"
		end
	end
end

class PipeGccWork < GccWork
	def gcc; ENV["MOSYNCDIR"] + "/bin/xgcc"; end
	def gccmode; "-S"; end
	
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
			@TARGET_PATH = @TARGETDIR + "/" + @BUILDDIR + filename
		end
		@TARGET = PipeTask.new(self, @TARGET_PATH, all_objects + llo, @FLAGS + @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
