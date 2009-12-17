#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = [".", "deflate/deflate", "deflate/host"]
	@EXTRA_INCLUDES = ["../../intlibs", "deflate/inc"]
	#@IGNORED_FILES =	["Emu.c", "BrewRebuild.c", "Peeper.c", "JavaCodeGen.c", "disas.c"]
	#@EXTRA_CFLAGS = " -Wno-strict-prototypes -Wno-missing-prototypes -Wno-old-style-definition" +
		#" -Wno-missing-noreturn -Wno-shadow -Wno-unreachable-code -Wno-write-strings -Wno-multichar" +
		#" -Wno-missing-format-attribute -D_CRT_SECURE_NO_DEPRECATE -DUSE_ZLIB -fno-strict-aliasing"
	# -Wno-unused-function
	#@LIBRARIES = ["z"]
	@NAME = "e32hack"
	
	setup
end

work.invoke
