#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ['src/net', 'src/base']
	if(USE_NEWLIB)
		@EXTRA_CFLAGS = " -DUSE_NEWLIB"
	end
	@EXTRA_LINKFLAGS = " -stacksize=128000 -datasize=2048000 -heapsize=1024000"
	@LIBRARIES = ['testify', 'mautil']
	@NAME = "autoTest"
end

work.invoke
