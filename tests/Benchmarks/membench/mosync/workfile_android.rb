#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil"]
	@EXTRA_INCLUDES = ["../../database_libs/mosync/include/"]
	@EXTRA_OBJECTS = [FileTask.new(self, "../../database_libs/mosync/lib/benchdb.lib")]
	@NAME = "MemBench"
	@PACK_PARAMETERS = ' -i /Applications/MoSync/etc/default.icon -m "Android/Android 2.x" --vendor MoSync -n MemBench --version 1.0 --permissions "File Storage,File Storage/Read,File Storage/Write,Internet Access,Vibration" --android-package com.mosync.app_MemBench --android-version-code 1'
end

work.invoke
