#!/usr/bin/ruby

require File.expand_path('../../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"
	@LIBRARIES = ["mautil", "maui"]
	@NAME = "MAUIex"
end

work.invoke
