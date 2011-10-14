#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.', 'Algorithms', 'Containers', 'Iterators', 'Strings', 'Utilities']
	@EXTRA_INCLUDES = ["#{mosyncdir}/include/newlib/stlport"]
	@EXTRA_CPPFLAGS = ' -Wno-missing-noreturn'
	@SPECIFIC_CFLAGS = {
		'Algorithms.cpp' => ' -Wno-unreachable-code',
		'Pairs.cpp' => ' -Wno-float-equal',
		'Example_using_bitset.cpp' => ' -Wno-extra',
	}
	@LIBRARIES = ['mautil', 'nativeui', 'stlport']
	@EXTRA_LINKFLAGS = standardMemorySettings(11)
	@NAME = 'HelloSTL'
end

Targets.setup

raise unless(USE_NEWLIB)

work.invoke
