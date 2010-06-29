#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

# install the kazlib headers separately.
class Kazlib < Work
	include MoSyncMod
	def setup
		@SOURCES = ["../kazlib"]
		@INSTALL_INCDIR = "kazlib"
		copyHeaders
	end
	def execute_clean
	end
end
Kazlib.new.invoke

mod = Module.new
mod.class_eval do
	def setup_base
		@SOURCES = ["."]
		@EXTRA_SOURCEFILES = ["../kazlib/dict.c", "../kazlib/hash.c"]
		@INSTALL_INCDIR = "MAUtil"
		@NAME = "mautil"
		@IGNORED_FILES = ["DomParser.cpp", "XMLDataProvider.cpp", "XPathTokenizer.cpp"]
		@IGNORED_HEADERS = ["DomParser.h", "XMLDataProvider.h", "XPathTokenizer.h", "Tokenizer.h", "ErrorListenable.h"]

		if(CONFIG == "")
			# broken compiler
			shared_specflags = {"CharInputC.c" => " -Wno-unreachable-code",
				"Graphics.c" => " -Wno-unreachable-code",
				"FrameBuffer.c" => " -Wno-unreachable-code"}
			if(@GCC_IS_V4)
				shared_specflags["String.cpp"] = " -Wno-strict-overflow"
			end
		else
			shared_specflags = {}
		end
		shared_specflags["dict.c"] = " -Wno-unreachable-code"
		shared_specflags["hash.c"] = " -Wno-unreachable-code"
		
		@NATIVE_SPECIFIC_CFLAGS = shared_specflags
		
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
