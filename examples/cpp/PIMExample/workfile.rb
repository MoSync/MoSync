#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil"]
	@LSTFILES = ["Resources/Resources.lst"]
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = "PIMExample"
end

work.invoke
