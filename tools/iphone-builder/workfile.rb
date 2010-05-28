#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@LOCAL_LIBS = ["filelist"]
	@EXTRA_CPPFLAGS = ""
	@NAME = "iphone-builder"
	@TARGETDIR = "."
end

work.invoke
