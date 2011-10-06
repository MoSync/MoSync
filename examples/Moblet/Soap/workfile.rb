#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil", "mtxml"]
	@EXTRA_LINKFLAGS = standardMemorySettings(8) unless(USE_NEWLIB)
	@NAME = "soap"
end

work.invoke
