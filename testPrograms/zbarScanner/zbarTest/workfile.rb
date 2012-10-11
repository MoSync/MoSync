#!/usr/bin/ruby

USE_NEWLIB=true

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@LSTX = 'res.lstx'
	@LIBRARIES = ['zbar']
	@EXTRA_INCLUDES = ['../zbar/include', "#{mosyncdir}/include/newlib/stlport"]
	@EXTRA_LINKFLAGS = standardMemorySettings(14)
	@NAME = 'zbarTest'
end

work.invoke
