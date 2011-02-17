#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncDll.new
work.instance_eval do 
	@SOURCES = ['src']
	common_libraries = ["OpenGL32", "GlU32"]
	@EXTRA_CFLAGS = ' -D__GL_EXPORTS -Wno-float-equal -Wno-unreachable-code'
	@EXTRA_INCLUDES = ['.']
	if(HOST == :win32) then
		@IGNORED_FILES = ['posix.c']
		@LIBRARIES = common_libraries
	else
		@IGNORED_FILES = ['win32.c']
	end

	
	@NAME = 'dgles'
end

work.invoke
