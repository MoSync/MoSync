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

# This file defines the class used for compiling MoSync programs.

require "#{File.dirname(__FILE__)}/pipe.rb"

MOSYNC_INCLUDE = "#{ENV['MOSYNCDIR']}/include"
MOSYNC_LIBDIR = "#{ENV['MOSYNCDIR']}/lib"

class PipeExeWork < PipeGccWork
	def setup
		@FLAGS = " -B"
		@EXTRA_INCLUDES = @EXTRA_INCLUDES.to_a + [MOSYNC_INCLUDE]
		default(:TARGETDIR, '.')
		super
	end
	def setup3(all_objects)
		# resource compilation
		if(!defined?(@LSTFILES))
			@LSTFILES = Dir[@SOURCES[0] + "/*.lst"].collect do |name| FileTask.new(self, name) end
		end
		@prerequisites << PipeResourceTask.new(self, "build/resources", @LSTFILES)
		
		# libs
		libs = (["mastd"] + @LIBRARIES).collect do |lib|
			FileTask.new(self, "#{MOSYNC_LIBDIR}/pipe/#{@CONFIG_NAME}/#{lib}.lib")
		end
		all_objects += libs
		super(all_objects)
	end
end
