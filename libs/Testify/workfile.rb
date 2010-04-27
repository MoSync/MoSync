#!/usr/bin/ruby
require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def setup_native
		setup_base
	end
	
	def setup_pipe
		setup_base
	end
	
	def setup_base
		@EXTRA_INCLUDES = ["inc"]
		@SOURCES = ["src", "inc"]
		@INSTALL_INCDIR = "testify"
		@NAME = "testify"
	end
end

MoSyncLib.invoke(mod)
