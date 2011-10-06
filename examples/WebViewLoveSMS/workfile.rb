#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')
require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_resources.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@PREREQUISITES = [BundleTask.new(self, './Resources/LocalFiles.bin', './LocalFiles')]
	@LSTFILES = ['Resources/Resources.lst']
	@LIBRARIES = ['mautil', 'josync', 'mafs', 'nativeui']
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'WebViewLoveSms'
end

work.invoke
