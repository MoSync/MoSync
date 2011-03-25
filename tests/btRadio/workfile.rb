#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_util.rb')
require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ['.']
	@LIBRARIES = ['bthprops']
	@NAME = 'btRadio'
end

work.invoke
