#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	if(CONFIG == "" && NATIVE_GCC_IS_V4)
		#broken compiler/stdlib
		native_specflags = {"conprint.c" => " -Wno-unreachable-code"}
	else
		native_specflags = {}
	end
	if(CONFIG="")
		pipe_specflags = {"strtod.c" => " -Wno-unreachable-code",
			"tlsf.c" => " -Wno-unreachable-code"}
		# broken compiler
		native_specflags["mastdlib.c"] = " -Wno-unreachable-code"
		native_specflags["mastring.c"] = " -Wno-unreachable-code"
		native_specflags["mavsprintf.c"] = " -Wno-unreachable-code"
	else
		pipe_specflags = {}
	end
	
	NATIVE_SPECIFIC_CFLAGS = {
		"madmath.c" => " -Wno-missing-declarations",
		"mavsprintf.c" => " -Wno-float-equal"}.merge(native_specflags, &HashMergeAdd)
	
	PIPE_SPECIFIC_CFLAGS = NATIVE_SPECIFIC_CFLAGS.merge({
		"intrinsics.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
		"madmath.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
		"maint64.c" => " -fno-strict-aliasing -Wno-missing-prototypes -Wno-missing-declarations",
		"strtod.c" => " -Wno-float-equal",
		"e_log.c" => " -Wno-float-equal",
		"s_atan.c" => " -fno-strict-aliasing",
		"e_atan2.c" => " -fno-strict-aliasing",
		"e_asin.c" => " -fno-strict-aliasing"}, &HashMergeAdd).merge(pipe_specflags, &HashMergeAdd)
	
	def setup_native
		@SOURCES = []
		@EXTRA_SOURCEFILES = ["conprint.c", "ma.c", "maassert.c", "mactype.c", "madmath.c",
			"mastdlib.c", "mastring.c", "matime.c", "mavsprintf.c", "maxtoa.c", "maheap.c"]
		@SPECIFIC_CFLAGS = NATIVE_SPECIFIC_CFLAGS
		
		@LOCAL_DLLS = ["mosync"]
		setup_base
	end
	
	def setup_pipe
		@SOURCES = [".", "../libsupc++"]
		@IGNORED_FILES = ["new_handler.cc"]
		@SPECIFIC_CFLAGS = PIPE_SPECIFIC_CFLAGS
		
		@EXTRA_OBJECTS = [FileTask.new(self, "crtlib.s")]
		setup_base
	end
	
	def setup_base
		@INSTALL_INCDIR = "."
		@IGNORED_HEADERS = ["math_private.h", "fdlibm.h"]
		@NAME = "mastd"
	end
end

MoSyncLib.invoke(mod)
