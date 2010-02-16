#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = [".", "deflate/deflate", "deflate/host"]
	@EXTRA_INCLUDES = ["../../intlibs", "deflate/inc"]
	@NAME = "e32hack"
	
	setup
end

work.invoke
