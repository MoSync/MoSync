#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')
require File.expand_path('../../rules/mosync_util.rb')

work = ExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["../../intlibs"]
	@LOCAL_LIBS = ["idl-common"]
	@NAME = "mx-config"
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
