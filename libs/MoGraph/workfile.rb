#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

work = PipeLibWork.new
work.instance_eval do
	def setup_native
		@LOCAL_DLLS = ["mosync", "mastd", "mautil"]
		setup_base
	end

	def setup_pipe
		@SOURCES = ['.']
		@HEADER_DIRS = ['.']
		@EXTRA_INCLUDES = ['..', mosync_include+'/stlport']
		@EXTRA_CPPFLAGS = ' -Wno-float-equal -Wno-unreachable-code -Wno-shadow -Wno-missing-noreturn'

		@INSTALL_INCDIR = 'MoGraph'
		@NAME = 'MoGraph'
	end
end

work.invoke
