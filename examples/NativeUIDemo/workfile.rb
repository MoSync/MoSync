#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@LSTFILES = ['Resources/Resources.lst']
	@LIBRARIES = ['mautil', 'maui', 'nativeui']
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'NativeUIDemo'
end

work.invoke
