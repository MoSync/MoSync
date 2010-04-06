#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	if(USE_NEWLIB)
		@EXTRA_LINKFLAGS = " -stacksize=32000 -datasize=128000"
		@EXTRA_CFLAGS = " -DUSE_NEWLIB"
	end
	@LIBRARIES = []
	@NAME = "wsprintf"
end

work.invoke
