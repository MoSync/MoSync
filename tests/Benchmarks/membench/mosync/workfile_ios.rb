#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil"]
	@EXTRA_INCLUDES = ["../../database_libs/mosync/include/"]
	@EXTRA_OBJECTS = [FileTask.new(self, "../../database_libs/mosync/lib/benchdb.lib")]
	@NAME = "MemBench"
	@PACK_PARAMETERS = ' --ios-bundle-id "com.MoSync.MemBench" --ios-sdk iphoneos4.3 --ios-xcode-target Release -i /Applications/MoSync/etc/default.icon'
end

work.invoke
