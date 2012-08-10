#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["linpack.cpp"]
	@LIBRARIES = ["mautil", "benchdb"]
	@NAME = "linpack"
	@PACK_PARAMETERS = ' --ios-bundle-id "com.MoSync.linpack" --ios-sdk iphoneos4.3 --ios-xcode-target Release -i /Applications/MoSync/etc/default.icon'
end

work.invoke
