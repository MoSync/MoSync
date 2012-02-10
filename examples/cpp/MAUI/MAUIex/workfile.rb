#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"
	@LIBRARIES = ["mautil", "maui"]
	@EXTRA_LINKFLAGS = standardMemorySettings(8) unless(USE_NEWLIB)
	@NAME = "MAUIex"
end

work.invoke
