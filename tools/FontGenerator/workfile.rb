#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = ""
	if(HOST == :win32) then
		@CUSTOM_LIBS = ["freeimage.lib"]
	else
		@LIBRARIES = ["freeimage"]
	end
	
	@NAME = "FontGenerator"
end

work.invoke
