#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil", "benchdb"]
	@EXTRA_LINKFLAGS = standardMemorySettings(13)
	@NAME = "GLBench"
	@PACK_PARAMETERS = ' --debug -i /Applications/MoSync/etc/default.icon -m "Android/Android 2.x" --vendor MoSync -n GLBench --version 1.0 --permissions "File Storage,File Storage/Read,File Storage/Write,Internet Access,Vibration" --android-package com.mosync.app_GLBench --android-version-code 1'
end

work.invoke
