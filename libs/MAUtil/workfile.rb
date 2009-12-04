#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	if(CONFIG == "" && !GCC_IS_V4)
		# broken compiler
		native_specflags = {"CharInputC.c" => " -Wno-unreachable-code",
			"Graphics.c" => " -Wno-unreachable-code",
			"FrameBuffer.c" => " -Wno-unreachable-code"}
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
		@IGNORED_FILES = ["DomParser.cpp", "XMLDataProvider.cpp", "XPathTokenizer.cpp"]
	end
end

MoSyncLib.invoke(mod)
