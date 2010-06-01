#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@LOCAL_LIBS = ["filelist"]
	@EXTRA_CFLAGS = " -Wno-missing-declarations "
	@NAME = "Bundle"
	@TARGETDIR = "."
end

work.invoke
