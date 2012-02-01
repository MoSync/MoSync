#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@LIBRARIES = ['mautil', 'nativeui']
	@LSTX = 'Res/res.lstx'
	@RES_PLATFORM = 'windows phone'
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'ResourceTest'
end

work.invoke
