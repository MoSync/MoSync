#!/usr/bin/ruby

require File.expand_path('../../../rules/mosync_exe.rb')
require File.expand_path('../../../rules/targets.rb')

server = PipeExeWork.new
server.instance_eval do
	@EXTRA_SOURCEFILES = ['server.cpp']
	@LIBRARIES = ['mautil']
	@NAME = 'btServer'
	@BUILDDIR_PREFIX = 'server_'
end

client = PipeExeWork.new
client.instance_eval do
	@EXTRA_SOURCEFILES = ['client.cpp']
	@LIBRARIES = ['mautil']
	@NAME = 'btClient'
	@BUILDDIR_PREFIX = 'client_'
end

target :default do
	server.invoke
end

target :client => :default do
	client.invoke
end

target :clean do
	server.invoke_clean
	client.invoke_clean
end

Targets.invoke
