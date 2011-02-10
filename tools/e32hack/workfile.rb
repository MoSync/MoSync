#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')
require File.expand_path('../../rules/mosync_util.rb')


work = ExeWork.new
work.instance_eval do
	@SOURCES = [".", "deflate/deflate", "deflate/host"]
	@EXTRA_INCLUDES = ["../../intlibs", "deflate/inc"]
	@NAME = "e32hack"
	
	@INSTALLDIR = mosyncdir + '/bin'
	
	setup
end

work.invoke
