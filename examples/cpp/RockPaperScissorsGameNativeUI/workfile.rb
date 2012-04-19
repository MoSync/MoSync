#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@LSTFILES = ['Resources/res.lst']
	@LIBRARIES = ['mautil']
	@NAME = 'RockPaperScissorsGameNativeUI'
end

work.invoke
