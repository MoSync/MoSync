#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ['src/net', 'src/base']
	if(USE_NEWLIB)
		@EXTRA_LINKFLAGS = " -stacksize=32000 -datasize=128000"
		@EXTRA_CFLAGS = " -DUSE_NEWLIB"
	end
	@LIBRARIES = ['testify', 'mautil']
	@NAME = "autoTest"
end

work.invoke
