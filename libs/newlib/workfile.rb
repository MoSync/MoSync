#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

default_const(:USE_NEWLIB, true)
if(!USE_NEWLIB)
	error "Must USE_NEWLIB!"
end

work = PipeLibWork.new
work.instance_eval do
	def copyHeaderDir(name)
		@INSTALL_INCDIR = name
		@HEADER_DIRS = ["libc/include/" + name]
		copyHeaders
	end
	
	def copyGlHeaders()
		@INSTALL_INCDIR = "GLES"
		@HEADER_DIRS = ["../MAStd/GLES"]
		copyHeaders
	end
	
	def copyGl2Headers()
		@INSTALL_INCDIR = "GLES2"
		@HEADER_DIRS = ["../MAStd/GLES2"]
		copyHeaders
	end	
	
	def setup_pipe
		@SOURCES = ["libc/sys/mosync", "libc/sys/mosync/libgcc", "../libsupc++", "libc/sys/mosync/quad",
			"libc/misc", "libc/unix", "libc/posix", "libc/locale", "libc/reent", "libc/stdio",
			"libc/search", "libc/stdlib", "libc/string", "libc/time", "libc/ctype", "libc/errno",
			"libm/math", "libm/common"]
		@EXTRA_INCLUDES = ["libc/include", "libc/sys/mosync", "libm/common"]

		@EXTRA_CFLAGS = " -DUSE_EXOTIC_MATH -Wno-float-equal -Wno-unreachable-code -Wno-sign-compare -Wno-old-style-definition"
		if(CONFIG=="")
			# buggy compiler, buggy libs... I won't fix them.
			@EXTRA_CFLAGS += " -Wno-uninitialized"
		end
		if(CONFIG=="debug")
			@EXTRA_CFLAGS += " -DMOSYNCDEBUG"
		end
		@SPECIFIC_CFLAGS = {
			"dtoa.c" => " -Wno-shadow -Wno-write-strings",
			"ldtoa.c" => " -Wno-shadow",
			"vfprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings -Wno-missing-declarations -Wno-missing-prototypes",
			"svfprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings -Wno-missing-declarations -Wno-missing-prototypes",
			"vfwprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings",
			"svfwprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings",
			"vfscanf.c" => " -Wno-shadow -Wno-missing-declarations -Wno-missing-prototypes",
			"svfscanf.c" => " -Wno-shadow -Wno-missing-declarations -Wno-missing-prototypes",
			"impure.c" => " -Wno-extra",
			"madmath.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
			"maint.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
			"machine.c" => " -Wno-missing-noreturn -D_COMPILING_NEWLIB",
			"gdtoa-gethex.c" => " -Wno-shadow",
			"strtod.c" => " -Wno-shadow",
			"wctomb.c" => " -Wno-shadow",
			"wctomb_r.c" => " -Wno-shadow",
			"sf_ldexp.c" => " -Wno-shadow",
			"s_ldexp.c" => " -Wno-shadow",
			"e_pow.c" => " -Wno-shadow",
			"ef_pow.c" => " -Wno-shadow",
			"s_floor.c" => " -Wno-shadow",
			"sf_floor.c" => " -Wno-shadow",
			"s_ceil.c" => " -Wno-shadow",
			"sf_ceil.c" => " -Wno-shadow",
			"ef_hypot.c" => " -Wno-shadow",
			"s_rint.c" => " -Wno-shadow",
			"s_lrint.c" => " -Wno-shadow",
			"s_llrint.c" => " -Wno-shadow",
			"sf_rint.c" => " -Wno-shadow",
			"sf_lrint.c" => " -Wno-shadow",
			"sf_llrint.c" => " -Wno-shadow",
			"s_modf.c" => " -Wno-shadow",
			"sf_modf.c" => " -Wno-shadow",
			"e_hypot.c" => " -Wno-shadow",
			"regexec.c" => " -Wno-char-subscripts",
			"regcomp.c" => " -Wno-char-subscripts",
			"mktemp.c" => " -DHAVE_MKDIR",
		}
		
		@IGNORED_FILES = ["engine.c"]
		
		@EXTRA_OBJECTS = [FileTask.new(self, "libc/sys/mosync/crtlib.s")]
		
		# copy subdirs
		copyHeaderDir("sys")
		copyHeaderDir("machine")
		copyGlHeaders()
		copyGl2Headers()

		@HEADER_DIRS = ["libc/include", "libc/sys/mosync"]
		@INSTALL_INCDIR = "."
		
		@NAME = "newlib"
	end
end

work.invoke
