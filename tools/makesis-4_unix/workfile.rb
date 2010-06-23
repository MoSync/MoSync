#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

class SisWork < MoSyncExe
	def init
		@SOURCES = ["src"]
		@EXTRA_INCLUDES = ["include", "src"]
		@IGNORED_FILES = ["makekeys.cpp", "dumptree.cpp", "dumpcontroller.cpp", "finddatetime.cpp"]
		@EXTRA_CPPFLAGS = " -Wno-shadow -Wno-unreachable-code"
		if(!@GCC_IS_V4 && CONFIG == "")	#buggy compiler, I think.
			@SPECIFIC_CFLAGS = {"crc.c" => " -Wno-unreachable-code"}
		end
		@LIBRARIES = ["z", "crypto"]
		@CUSTOM_LIBS = ["libeay32.lib"]
	end
end

makesis = SisWork.new
makesis.instance_eval do
	init
	@IGNORED_FILES += ["signsis.cpp"]
	@NAME = "makesis-4"
	@INSTALLDIR = mosyncdir + '/bin'
end

signsis = SisWork.new
signsis.instance_eval do
	init
	@IGNORED_FILES += ["makesis.cpp"]
	@NAME = "signsis-4"
	@INSTALLDIR = mosyncdir + '/bin'
end

makesis.invoke
signsis.invoke
