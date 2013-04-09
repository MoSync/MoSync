#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')
require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_resources.rb')

work = PipeExeWork.new
work.instance_eval do
	@PREREQUISITES = [
		DirTask.new(self, 'Resources'),
		BundleTask.new(self, './Resources/LocalFiles.bin', './LocalFiles'),
	]
	@SOURCES = ['.']
	@EXTRA_CPPFLAGS = ' -Wno-shadow'
	@LIBRARIES = ['mautil', 'mafs', 'nativeui']
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'AudioApiTest'
end

work.invoke
