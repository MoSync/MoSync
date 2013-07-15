#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["..","../../tools/ReleasePackageBuild/build_package_tools/include/"]
	@NAME = "mustache"
end

work.invoke
