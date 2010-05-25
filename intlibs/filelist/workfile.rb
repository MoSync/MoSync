#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do 
	@SOURCES = []
	if(HOST == :win32)
		@EXTRA_SOURCEFILES = ["filelist-win32.c"]
	elsif(HOST == :linux)
		@EXTRA_SOURCEFILES = ["filelist-linux.c"]
	elsif(HOST == :darwin)
		@EXTRA_SOURCEFILES = ["filelist-linux.c"]
	else
		error "Unsupported platform"
	end
	@EXTRA_INCLUDES = [".."]
	@NAME = "filelist"
end

work.invoke
