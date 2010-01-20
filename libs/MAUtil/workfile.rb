#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

class Kazlib
	include MoSyncMod
	def initialize
		@SOURCES = ["../kazlib"]
		@INSTALL_INCDIR = "kazlib"
	end
end
Kazlib.new.copyHeaders

mod = Module.new
mod.class_eval do
	def setup_base
		@SOURCES = ["."]
		@EXTRA_SOURCEFILES = ["../kazlib/dict.c", "../kazlib/hash.c"]
		@INSTALL_INCDIR = "MAUtil"
		@NAME = "mautil"
		@IGNORED_FILES = ["DomParser.cpp", "XMLDataProvider.cpp", "XPathTokenizer.cpp"]
		
		if(CONFIG == "")
			# broken compiler
			shared_specflags = {"CharInputC.c" => " -Wno-unreachable-code",
				"Graphics.c" => " -Wno-unreachable-code",
				"FrameBuffer.c" => " -Wno-unreachable-code"}
		else
			shared_specflags = {}
		end
		shared_specflags["dict.c"] = " -Wno-unreachable-code"
		shared_specflags["hash.c"] = " -Wno-unreachable-code"
		if(CONFIG == "" && NATIVE_GCC_IS_V4)
			native_specflags = {"String.cpp" => " -Wno-strict-overflow"}
		else
			native_specflags = {}
		end
		
		@NATIVE_SPECIFIC_CFLAGS = native_specflags.merge(shared_specflags)
		
		@PIPE_SPECIFIC_CFLAGS = shared_specflags
	end
	
	def setup_native
		setup_base
		@SPECIFIC_CFLAGS = @NATIVE_SPECIFIC_CFLAGS
		@LOCAL_DLLS = ["mosync", "mastd"]
	end
	
	def setup_pipe
		setup_base
		@SPECIFIC_CFLAGS = @PIPE_SPECIFIC_CFLAGS
	end
end

MoSyncLib.invoke(mod)
