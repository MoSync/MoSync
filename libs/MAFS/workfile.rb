#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def setup_native
		@LOCAL_DLLS = ["mosync", "mastd"]
		if(@GCC_IS_V44)
			@EXTRA_CFLAGS = " -D_STDIO_H"
		end
		setup_base
	end
	
	def setup_pipe
		setup_base
	end
	
	def setup_base
		if(@CONFIG == "")
			# broken compiler
			@SPECIFIC_CFLAGS = {"File.c" => " -Wno-unreachable-code"}
		else
			@SPECIFIC_CFLAGS = {}
		end
		@SOURCES = ["."]
		@INSTALL_INCDIR = "MAFS"
		@NAME = "mafs"
	end
end

MoSyncLib.invoke(mod)
