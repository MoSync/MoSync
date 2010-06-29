#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@LOCAL_LIBS = ["filelist"]
	@NAME = "iphone-builder"
	@TARGETDIR = "."
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
