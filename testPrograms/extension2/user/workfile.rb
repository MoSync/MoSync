#!/usr/bin/ruby

require File.expand_path('../../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@EXTENSIONS = [['../test.idl', 'Test']]
	@SOURCES = ['.']
	@EXTRA_INCLUDES = ['build']
	@EXTRA_CPPFLAGS = ''
	@NAME = "ext2_user"
end

work.invoke
