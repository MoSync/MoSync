#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@NAME = "nbuild"
	@TARGETDIR = "."
	@SPECIFIC_CFLAGS = {
        "nbuild.cpp" => " -Wno-missing-noreturn",
	}
    if ( HOST == :darwin )
        # Objective-C++ compiler
        @EXTRA_CPPFLAGS = " -Wno-shadow -Wno-missing-prototypes"
    end
	@LOCAL_LIBS = ["filelist", "profiledb", "mustache"]
	if(HOST==:linux || HOST==:darwin)
		@LIBRARIES = ["expat"]
	else	# win32
		@CUSTOM_LIBS = ["libexpat.lib"]
	end
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
