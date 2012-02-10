#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil"]
	@NAME = "ΩΩΩ"
	@PACK_ANDROID_PACKAGE = "com.mosync.test.unicode"
	@PACK_PARAMETERS = ' --s60v3uid e9834701 --s60v2uid 0317d49c -t platform --wp-config Release'
end

work.invoke
