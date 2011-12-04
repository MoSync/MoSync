#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_INCLUDES = [".."]
	@NAME = "profiledb"
end

work.invoke
