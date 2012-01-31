#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ['.', 'GUI', 'Application']
	#@EXTRA_CPPFLAGS = " -Wno-shadow"
	@LIBRARIES = ['mautil', 'maui', 'facebook', 'yajl', 'nativeui']
	@EXTRA_LINKFLAGS = ' -datasize=1024000 -heapsize=386000 -stacksize=64000'
	@NAME = 'FacebookDemo'
end

work.invoke
