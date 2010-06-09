#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def setup_native
		setup_base
		@SOURCES = ["libgcc"]
		@EXTRA_SOURCEFILES = ["conprint.c", "ma.c", "maassert.c", "mactype.c", "madmath.c",
			"mastdlib.c", "mastring.c", "matime.c", "mavsprintf.c", "maxtoa.c", "maheap.c"]
		@SPECIFIC_CFLAGS = @native_specific_cflags
		
		@LOCAL_DLLS = ["mosync"]
	end
	
	def setup_pipe
		setup_base
		@SOURCES = [".", "../libsupc++", "libgcc"]
		@EXTRA_INCLUDES = []
		@IGNORED_FILES = ["new_handler.cc"]
		@SPECIFIC_CFLAGS = @pipe_specific_cflags
		
		@EXTRA_OBJECTS = [FileTask.new(self, "crtlib.s")]
	end
	
	def setup_base
		if(CONFIG == "" && @GCC_IS_V4)
			#broken compiler/stdlib
			native_specflags = {"conprint.c" => " -Wno-unreachable-code"}
			if(@GCC_IS_V44)
				native_specflags["conprint.c"] += " -Wno-inline"
			end
		else
			native_specflags = {}
		end
		if(CONFIG == "")
			pipe_specflags = {"strtod.c" => " -Wno-unreachable-code",
				"tlsf.c" => " -Wno-unreachable-code"}
			# broken compiler
			native_specflags["mastdlib.c"] = " -Wno-unreachable-code"
			native_specflags["mastring.c"] = " -Wno-unreachable-code"
			native_specflags["mawstring.c"] = " -Wno-unreachable-code"
			native_specflags["mavsprintf.c"] = " -Wno-unreachable-code"
			native_specflags["mawvsprintf.c"] = " -Wno-unreachable-code"
		else
			pipe_specflags = {}
		end
		
		@native_specific_cflags = {
			"madmath.c" => " -Wno-missing-declarations",
			"mavsprintf.c" => " -Wno-float-equal",
			"mawvsprintf.c" => " -Wno-float-equal",
			}.merge(native_specflags, &HashMergeAdd)
		
		@pipe_specific_cflags = @native_specific_cflags.merge({
			"intrinsics.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
			"madmath.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
			"maint64.c" => " -fno-strict-aliasing -Wno-missing-prototypes -Wno-missing-declarations",
			"libgcc2.c" => " -Wno-unreachable-code",
			"strtod.c" => " -Wno-float-equal",
			"e_log.c" => " -Wno-float-equal",
			"s_atan.c" => " -fno-strict-aliasing",
			"e_atan2.c" => " -fno-strict-aliasing",
			"e_asin.c" => " -fno-strict-aliasing",
			"mastdlib.c" => " -Wno-deprecated-declarations",
			}, &HashMergeAdd).merge(pipe_specflags, &HashMergeAdd)
		
		@INSTALL_INCDIR = "."
		@IGNORED_HEADERS = ["math_private.h", "fdlibm.h"]
		@NAME = "mastd"
	end
end

MoSyncLib.invoke(mod)
