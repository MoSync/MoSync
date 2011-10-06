#!/usr/bin/ruby

require File.expand_path("../../rules/native_lib.rb")

work = NativeLibWork.new
work.instance_eval do
	@SOURCES = ['.']
	@NAME = 'sqlite'
	@EXTRA_CFLAGS = ' -D_LIB -Wno-float-equal -Wno-error'
end

work.invoke
