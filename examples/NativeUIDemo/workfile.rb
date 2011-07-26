#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.', 'UIWrapper']
	@LSTFILES = ['Resources/Resources.lst']
	@LIBRARIES = ['mautil', 'maui']
	@NAME = 'NativeUIDemo'
end

work.invoke
