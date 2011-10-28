#!/usr/bin/ruby

require File.expand_path '../../rules/host.rb'
require File.expand_path '../../rules/exe.rb'

@work = ExeWork.new
@work.instance_eval do
	@SOURCES = ['.', 'Helper']
	@EXTRA_CFLAGS = ' -DIS_DEVELOPMENT_VERSION -Wno-declaration-after-statement'

	if(HOST == :win32)
		@LIBRARIES = ['ssleay32', 'libeay32']
		@LIBRARIES << 'wsock32'
		#@EXTRA_INCLUDES = ['/mingw/msys/1.0/include']
		#@EXTRA_LINKFLAGS = ' -L/mingw/msys/1.0/lib'
		#@EXTRA_INCLUDES = ['/OpenSSL-Win64/include']
		#@EXTRA_LINKFLAGS = ' -L/OpenSSL-Win64/lib'
	elsif(HOST == :linux || HOST == :darwin)
		@LIBRARIES = ['ssl']
	else
		error "Unsupported platform"
	end

	@NAME = 'iphone-apns'
	@TARGETDIR = '.'
end

def cmd; @work.target; end

target :default do
	@work.invoke
end

target :run => :default do
	sh cmd
end

target :gdb => :default do
	sh "gdb --args #{cmd}"
end

Targets.invoke
