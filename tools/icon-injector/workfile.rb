#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"	
	@NAME = "icon-injector"
	if(HOST==:linux || HOST==:darwin)
		@IGNORED_FILES = ["WinmobileInjector.cpp", "ErrorCheck.cpp", "IconFileLoader.cpp"]
	end
	@LIBRARIES = ["expat"]
end

work.invoke
