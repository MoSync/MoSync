#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_SOURCEFILES = ["../../runtimes/cpp/core/sld.cpp"]
	@EXTRA_INCLUDES = ["../../runtimes/cpp/core"]
	@EXTRA_CPPFLAGS = " -fno-exceptions -Wno-unreachable-code"
	if(!@GCC_IS_V4)	#buggy compiler
		if(CONFIG == "")
			@SPECIFIC_CFLAGS["stabs_types.cpp"] = " -Wno-uninitialized"
		end
	end
	@NAME = "stabs"
end

gen = FileTask.new(work, "stabs_builtins_generated.h")
gen.instance_eval do
	@prerequisites = [FileTask.new(work, "typeGen.rb"), FileTask.new(work, "types.rb")]
	def execute
		sh "ruby typeGen.rb"
	end
end

gen.invoke
work.invoke
