#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')
require File.expand_path('../../rules/mosync_util.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = ""
	
	@NAME = "updater"
	
	@INSTALLDIR = mosyncdir + '/bin'
	
end

work.invoke
