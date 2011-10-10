#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.', 'UIFramework']
	@EXTRA_CPPFLAGS = ' -Wno-shadow -Wno-float-equal'
	@LSTFILES = ['res/res.lst']
	@LIBRARIES = ['mautil']
	@NAME = 'Graphun'
end

work.invoke if(USE_NEWLIB)
