#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')
require File.expand_path('../../rules/mosync_util.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = ["src"]
	@EXTRA_INCLUDES = ["include"]
	@EXTRA_CPPFLAGS = " -Wno-shadow -Wno-unreachable-code"
	@LIBRARIES = ["z"]

	@NAME = "makesis-200"
	
	@INSTALLDIR = mosyncdir + '/bin'
	
end

work.invoke
