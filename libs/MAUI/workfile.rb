#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def setup_native
		@LOCAL_DLLS = ["mosync", "mastd", "mautil"]
		setup_base
	end
	
	def setup_pipe
		setup_base
	end
	
	def setup_base
		@SOURCES = ["."]
		@EXTRA_CPPFLAGS = " -Wno-shadow"	#TODO, low-prio: enable -Wshadow
		@INSTALL_INCDIR = "MAUI"
		@NAME = "maui"
	end
end

MoSyncLib.invoke(mod)
