#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"
	@LSTFILES = ["Res/res.lst"]
	@LIBRARIES = ["mautil"]
	@NAME = "MoTris"
	@PACK_PARAMETERS = ' --s60v3uid E3450F2C --s60v2uid 00297B7B'
end

work.invoke
