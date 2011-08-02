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
require "#{File.dirname(__FILE__)}/targets.rb"

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

# Packs a MoSync program for installation.
# resource can be nil. all other parameters must be valid.
class MoSyncPackTask < Task
	def initialize(work, tempdir, buildpath, model, program, resource, name)
		super(work)
		@model = model
		@program = program
		@resource = resource
		@packpath = buildpath + model
		@name = name
		@tempdir = tempdir
		@prerequisites = [@program, DirTask.new(work, @packpath)]
		@prerequisites << @resource if(@resource)
	end
	def execute
		if(@resource)
			r = File.expand_path(@resource)
			resArg = " -r \"#{r}\""
		end
		p = File.expand_path(@program)
		d = File.expand_path(@packpath)
		FileUtils.cd(@tempdir, :verbose => true) do
			sh "#{mosyncdir}/bin/package -p \"#{p}\"#{resArg} -m \"#{@model}\""+
				" -d \"#{d}\" -n \"#{@name}\" --vendor MoSync"
		end
	end
end

class PipeExeWork < PipeGccWork
	def setup
		default(:TARGETDIR, '.')
		set_defaults
		@buildpath = @TARGETDIR + "/" + @BUILDDIR
		@SLD = @buildpath + "sld.tab"
		stabs = @buildpath + "stabs.tab"
		@FLAGS = " -sld=#{@SLD} -stabs=#{stabs} -B"
		@EXTRA_INCLUDES = @EXTRA_INCLUDES.to_a +
			[mosync_include, "#{mosyncdir}/profiles/vendors/MoSync/Emulator"]
		super
	end
	def setup3(all_objects, have_cppfiles)
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
			@resourceTask = PipeResourceTask.new(self, "build/resources", lstTasks)
			@prerequisites << @resourceTask
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
		
		super
		
		if(ELIM)
			@TARGET.extend(PipeElimTask)
		end
		if(defined?(PACK))
			@prerequisites = [@TARGET = MoSyncPackTask.new(self, @BUILDDIR_BASE, @buildpath,
				PACK, @TARGET, @resourceTask, @NAME)]
		end
	end
	def emuCommandLine
		if(@resourceTask)
			resArg = " -resource #{@resourceTask}"
		end
		return "#{mosyncdir}/bin/MoRE -program #{@TARGET} -sld #{@SLD}#{resArg}#{@EXTRA_EMUFLAGS}"
	end
	def run
		# run the emulator
		sh emuCommandLine
	end
	def gdb
		# debug the emulator
		sh "gdb --args #{emuCommandLine}"
	end
	def invoke
		super
		# If you invoke a work without setting up any targets,
		# we will check for the "run" goal here.
		if(Targets.size == 0)
			Targets.setup
			if(Targets.goals.include?(:run))
				self.run
				return
			end
			if(Targets.goals.include?(:gdb))
				self.gdb
				return
			end
		end
	end
end
