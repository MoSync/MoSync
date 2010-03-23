#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"
	@EXTRA_LINKFLAGS = " -stacksize=18000"
	@LIBRARIES = ["mautil", "mtxml"]
	@NAME = "mtxMoblet"
end

work.invoke
