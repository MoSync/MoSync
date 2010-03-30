#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CFLAGS = " -Wno-strict-prototypes -Wno-missing-prototypes -Wno-old-style-definition -Wno-missing-noreturn -Wno-unreachable-code"
	@NAME = "protobuild"
end

work.invoke
