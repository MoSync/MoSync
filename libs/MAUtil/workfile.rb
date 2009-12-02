#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

module mod
	if(CONFIG == "" && !GCC_IS_V4)
		# broken compiler
		native_specflags = {}
	else
		native_specflags = {}
	end
	
	NATIVE_SPECIFIC_CFLAGS = {}.merge(native_specflags)
	
	PIPE_SPECIFIC_CFLAGS = NATIVE_SPECIFIC_CFLAGS.merge({})
	
	def setup_native
		@SPECIFIC_CFLAGS = NATIVE_SPECIFIC_CFLAGS
		@LOCAL_DLLS = ["mosync", "mastd"]
		setup_base
	end
	
	def setup_pipe
		@SPECIFIC_CFLAGS = PIPE_SPECIFIC_CFLAGS
		setup_base
	end
	
	def setup_base
		@SOURCES = ["."]
		@INSTALL_INCDIR = "MAUtil"
		@NAME = "mautil"
	end
end

MoSyncLib.invoke(mod)
