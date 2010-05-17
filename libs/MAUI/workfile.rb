#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def copyHeaderDir(name)
		@INSTALL_INCDIR = "MAUI/" + name
		@HEADER_DIRS = [name]
		copyHeaders
	end

	def setup_native
		@LOCAL_DLLS = ["mosync", "mastd", "mautil", "mtxml"]
		setup_base
	end
	
	def setup_pipe
		setup_base
	end
	
	def setup_base
		@SOURCES = [".", "Keyboard", "Keyboard/Resources", "Keyboard/Parser/", "Keyboard/Util/"]
		@EXTRA_CPPFLAGS = " -Wno-shadow"	#TODO, low-prio: enable -Wshadow
		@INSTALL_INCDIR = "MAUI"
		@NAME = "maui"
		
		copyHeaderDir("Keyboard")
		copyHeaderDir("Keyboard/Parser")
		copyHeaderDir("Keyboard/Util")
	end
end

MoSyncLib.invoke(mod)
