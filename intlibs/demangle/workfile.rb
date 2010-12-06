#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do 
	@SOURCES = ["."]
	
	@EXTRA_CFLAGS = " -DHAVE_STDLIB_H -DHAVE_STRING_H";
	
	@SPECIFIC_CFLAGS = {
		"cp-demangle.c" => " -Wno-shadow -Wno-unreachable-code -Wno-inline"
	}

	@NAME = "demangle"
end

work.invoke
