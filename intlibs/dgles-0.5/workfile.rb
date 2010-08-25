#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncDll.new
work.instance_eval do 
	@SOURCES = ['src']
	@EXTRA_CFLAGS = ' -D__GL_EXPORTS -Wno-float-equal -Wno-unreachable-code'
	@EXTRA_INCLUDES = ['.']
	if(HOST == :win32) then
		@IGNORED_FILES = ['posix.c']
	else
		@IGNORED_FILES = ['win32.c']
	end

	
	@NAME = 'dgles'
end

work.invoke
