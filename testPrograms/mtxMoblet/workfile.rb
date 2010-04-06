#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"
	if(USE_NEWLIB)
		@EXTRA_LINKFLAGS = " -stacksize=32000 -datasize=128000"
	else
		@EXTRA_LINKFLAGS = " -stacksize=18000"
	end
	@LIBRARIES = ["mautil", "mtxml"]
	@NAME = "mtxMoblet"
end

work.invoke
