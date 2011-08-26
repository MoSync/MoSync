#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do 
	@SOURCES = []
	if(HOST == :win32)
		@EXTRA_SOURCEFILES = ["dll-win32.cpp"]
	elsif(HOST == :linux)
		@EXTRA_SOURCEFILES = ["dll-unix.cpp"]
	elsif(HOST == :darwin)
		@EXTRA_SOURCEFILES = ["dll-unix.cpp"]
	else
		error "Unsupported platform"
	end
	@NAME = "dll"
end

work.invoke
