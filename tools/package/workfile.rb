#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@NAME = "package"
	@TARGETDIR = "."
	@SPECIFIC_CFLAGS = {
                "selectPackager.cpp" => " -Wno-missing-noreturn",
	}
	@LOCAL_LIBS = ["filelist"]
	if(HOST==:linux || HOST==:darwin)
		@IGNORED_FILES = ["WinmobileInjector.cpp", "ErrorCheck.cpp", "IconFileLoader.cpp"]
		@LIBRARIES = ["expat"]
	else	# win32
		@CUSTOM_LIBS = ["libexpat.lib"]
	end
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
