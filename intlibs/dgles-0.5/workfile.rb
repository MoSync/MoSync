#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncDll.new
work.instance_eval do 
	@SOURCES = ['src']
	@EXTRA_CFLAGS = ' -D__GL_EXPORTS -Wno-float-equal'
	@EXTRA_INCLUDES = ['.']
	@IGNORED_FILES = ['posix.c']
	@NAME = 'dgles'
end

work.invoke
