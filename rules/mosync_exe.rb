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
require "#{File.dirname(__FILE__)}/mosync_util.rb"

class PipeExeWork < PipeGccWork
	def setup
		default(:TARGETDIR, '.')
		set_defaults
		sld = @TARGETDIR + "/" + @BUILDDIR + "sld.tab"
		stabs = @TARGETDIR + "/" + @BUILDDIR + "stabs.tab"
		@FLAGS = " -sld=#{sld} -stabs=#{stabs} -B"
		@EXTRA_INCLUDES = @EXTRA_INCLUDES.to_a +
			[mosync_include, "#{mosyncdir}/profiles/vendors/MobileSorcery/Emulator"]
		super
	end
	def setup3(all_objects)
		# resource compilation
		if(!defined?(@LSTFILES))
			if(@SOURCES[0])
				@LSTFILES = Dir[@SOURCES[0] + "/*.lst"]
			else
				@LSTFILES = []
			end
		end
		if(@LSTFILES.size > 0)
			lstTasks = @LSTFILES.collect do |name| FileTask.new(self, name) end
			@prerequisites << PipeResourceTask.new(self, "build/resources", lstTasks)
		end
		if(USE_NEWLIB)
			default(:DEFAULT_LIBS, ["newlib"])
		else
			default(:DEFAULT_LIBS, ["mastd"])
		end
		
		# libs
		libs = (@DEFAULT_LIBS + @LIBRARIES).collect do |lib|
			FileTask.new(self, "#{mosync_libdir}/#{@BUILDDIR_NAME}/#{lib}.lib")
		end
		all_objects += libs
		super(all_objects)
	end
end
