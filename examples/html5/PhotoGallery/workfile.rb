#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@LSTFILES = ['Resources/Resources.lst']
	@LIBRARIES = ['mautil', 'nativeui', 'mafs', 'wormhole', 'yajl', 'notification']
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'PhotoGallery'
end

work.invoke
