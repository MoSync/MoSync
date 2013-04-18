#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.', 'Screens']
	@LIBRARIES = ['mautil', 'nativeui']
	@LSTX = 'Res/res.lstx'
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'CameraDemo'
end

work.invoke
