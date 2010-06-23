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
	
	def setup_pipe
		@SOURCES = ["libc/sys/mosync", "libc/sys/mosync/libgcc", "../libsupc++", "libc/locale", "libc/reent", "libc/stdio",
			"libc/search", "libc/stdlib", "libc/string", "libc/time", "libc/ctype", "libc/errno",
			"libm/math", "libm/common"]
		@EXTRA_INCLUDES = ["libc/include", "libc/sys/mosync", "libm/common"]

		@EXTRA_CFLAGS = " -Wno-float-equal -Wno-unreachable-code -Wno-sign-compare"
		if(CONFIG=="")
			# buggy compiler, buggy libs... I won't fix them.
			@EXTRA_CFLAGS += " -Wno-uninitialized"
		end
		@SPECIFIC_CFLAGS = {
			"strcasestr.c" => " -Wno-old-style-definition",
			"dtoa.c" => " -Wno-write-strings",
			"ldtoa.c" => " -Wno-shadow",
			"vfprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings -Wno-missing-declarations -Wno-missing-prototypes",
			"svfprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings -Wno-missing-declarations -Wno-missing-prototypes",
			"vfwprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings",
			"svfwprintf.c" => " -Wno-shadow -Wno-missing-format-attribute -Wno-write-strings",
			"vfscanf.c" => " -Wno-shadow -Wno-missing-declarations -Wno-missing-prototypes",
			"svfscanf.c" => " -Wno-shadow -Wno-missing-declarations -Wno-missing-prototypes",
			"impure.c" => " -Wno-extra",
			"madmath.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
			"sf_infinity.c" => " -Wno-old-style-definition",
			"s_infinity.c" => " -Wno-old-style-definition",
			"maint.c" => " -Wno-missing-prototypes -Wno-missing-declarations",
			"machine.c" => " -Wno-missing-noreturn -D_COMPILING_NEWLIB",
			"gdtoa-gethex.c" => " -Wno-shadow",
			"strtod.c" => " -Wno-shadow",
			"wctomb.c" => " -Wno-shadow",
			"wctomb_r.c" => " -Wno-shadow",
			"sf_ldexp.c" => " -Wno-shadow",
			"s_ldexp.c" => " -Wno-shadow",
		}
		
		@EXTRA_OBJECTS = [FileTask.new(self, "libc/sys/mosync/crtlib.s")]
		
		# copy subdirs
		copyHeaderDir("sys")
		copyHeaderDir("machine")

		@HEADER_DIRS = ["libc/include", "libc/sys/mosync"]
		@INSTALL_INCDIR = "."
		
		@NAME = "newlib"
	end
end

work.invoke
