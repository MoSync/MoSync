#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@LOCAL_LIBS = ["filelist"]
	#if ( HOST == :darwin )
	#	# Objective-C++ compiler
	#	@EXTRA_CPPFLAGS = " -Wno-shadow -Wno-missing-prototypes"
	#end

	@NAME = "rescomp"
	if(HOST==:linux || HOST==:darwin)
		@IGNORED_FILES = ["WinmobileInjector.cpp", "ErrorCheck.cpp", "IconFileLoader.cpp"]
		@LIBRARIES = ["expat"]
	elsif(HOST==:win32)
		@CUSTOM_LIBS = ["libexpat.lib"]
	else
		raise 'Unsupported host!'
	end

	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
