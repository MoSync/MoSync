#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@EXTRA_LINKFLAGS = standardMemorySettings(14)
	@NAME = 'reload'
end

work.invoke
