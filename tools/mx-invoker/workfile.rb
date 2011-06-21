#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["../../intlibs"]
	@LOCAL_LIBS = ["idl-common"]
	@NAME = "mx-invoker"
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
