#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ['.']
	@EXTRA_SOURCEFILES = [
		'../../runtimes/cpp/base/FileStream.cpp',
		'../../runtimes/cpp/base/ThreadPool.cpp',
		'../../runtimes/cpp/platforms/sdl/FileImpl.cpp',
		'../../runtimes/cpp/platforms/sdl/ThreadPoolImpl.cpp',
	]
	@EXTRA_INCLUDES = ['../../intlibs', '../../runtimes/cpp/base', '../../runtimes/cpp/platforms/sdl']
	@LOCAL_LIBS = ['mosync_log_file', 'mosync_bluetooth']

	common_libraries = ['SDL', 'SDLmain']
	if(HOST == :win32) then
		@CUSTOM_LIBS = common_libraries.collect do |lib| "#{lib}.lib" end +
			['libirprops.a', 'libuuid.a']
		@LIBRARIES = ['wsock32', 'ws2_32']
	elsif(HOST == :linux) then
		@LIBRARIES = common_libraries + ['bluetooth']
		@EXTRA_SOURCEFILES << '../../runtimes/cpp/platforms/sdl/mutexImpl.cpp'
	elsif(HOST == :darwin)
		@LIBRARIES = common_libraries
		@EXTRA_SOURCEFILES << '../../runtimes/cpp/platforms/sdl/mutexImpl.cpp'
	else
		error 'Unsupported platform'
	end

	@NAME = 'unitTestSocketServer'
end

target :default do
	work.invoke
end

target :clean do
	work.setup
	work.execute_clean
end

target :run => :default do
	sh work.target
end

Targets.invoke
