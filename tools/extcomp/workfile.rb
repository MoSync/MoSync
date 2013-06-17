#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@NAME = "extcomp"
	@TARGETDIR = "."
    if ( HOST == :darwin )
        # Objective-C++ compiler
        @EXTRA_CPPFLAGS = " -Wno-shadow -Wno-missing-prototypes"
    end
	@LOCAL_LIBS = ["idl-common", "filelist", "profiledb"]

	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke