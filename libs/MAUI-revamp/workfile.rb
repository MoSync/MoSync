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
		@INSTALL_INCDIR = "maui-revamp/MAUI"
		@IGNORED_HEADERS = ["DefaultSkin.h"]
		@IGNORED_FILES = ["test.cpp"]
		@NAME = "maui-revamp"
	end
end

MoSyncLib.invoke(mod)
