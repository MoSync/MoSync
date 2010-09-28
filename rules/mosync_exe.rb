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

module PipeElimTask
	def execute
		execFlags
		# pipe-tool may output an empty file and then fail.
		begin
			sh "#{mosyncdir}/bin/pipe-tool -elim#{cFlags}"
			tarDir = @work.instance_variable_get(:@TARGETDIR) + "/" + @work.instance_variable_get(:@BUILDDIR)
			tarRebuild = tarDir + 'rebuild.se'
			tarSld = tarDir + 'slde.tab'
			FileUtils.mv('rebuild.s', tarRebuild)	# clean up cwd.
			FileUtils.rm_f(@NAME)	# make sure we know about any silent fail.
			sh "#{mosyncdir}/bin/pipe-tool#{@FLAGS} -sld=#{tarSld} #{@NAME} #{tarRebuild}"
		rescue => e
			FileUtils.rm_f('rebuild.s')
			FileUtils.rm_f(@NAME)
			raise
		end
		if(!File.exist?(@NAME))
			error "Pipe-tool failed silently!"
		end
	end
end

class PipeExeWork < PipeGccWork
	def setup
		default(:TARGETDIR, '.')
		set_defaults
		@SLD = @TARGETDIR + "/" + @BUILDDIR + "sld.tab"
		stabs = @TARGETDIR + "/" + @BUILDDIR + "stabs.tab"
		@FLAGS = " -sld=#{@SLD} -stabs=#{stabs} -B"
		@EXTRA_INCLUDES = @EXTRA_INCLUDES.to_a +
			[mosync_include, "#{mosyncdir}/profiles/vendors/MoSync/Emulator"]
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
			FileTask.new(self, "#{mosync_libdir}/#{@COMMON_BUILDDIR_NAME}/#{lib}.lib")
		end
		all_objects += libs
		
		super(all_objects)
		
		if(ELIM)
			@TARGET.extend(PipeElimTask)
		end
	end
end
