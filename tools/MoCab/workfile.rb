#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = ["lcab"]
	@NAME = "lcab"
	@TARGETDIR = "."
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
