#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CFLAGS = " -Wno-unreachable-code"
	if(USE_NEWLIB)
		@EXTRA_LINKFLAGS = " -datasize=256000 -heapsize=128000 -stacksize=16000"
	end
	@NAME = "MAStx"
end

work.invoke
