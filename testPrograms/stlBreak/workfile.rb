#!/usr/bin/ruby
USE_NEWLIB = true

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	def mosync_include
		'.'
	end
	@EXTRA_INCLUDES = ["#{mosyncdir}/include/newlib/", "#{mosyncdir}/include/newlib/stlport"]
	@EXTRA_CPPFLAGS = ' -Wno-missing-noreturn'
	@SPECIFIC_CFLAGS = {
	}
	@LIBRARIES = ['mautil', 'stlport']
	#@DEFAULT_LIBS = []
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'stlBreak'
end

Targets.setup

#raise unless(USE_NEWLIB)

work.invoke
