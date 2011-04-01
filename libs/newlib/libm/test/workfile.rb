#!/usr/bin/ruby

USE_NEWLIB = true

require File.expand_path('../../../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do 
	@SOURCES = ['.']
	@IGNORED_FILES = Dir['y*'] + Dir['j*'] + Dir['gamma*'] + ['dcvt.c', 'test_ieee.c']
	@EXTRA_CFLAGS = ' -Wno-all -Wno-extra -Wno-missing-format-attribute -Wno-return-type -Wno-nested-externs'+
		' -Wno-missing-declarations -Wno-strict-prototypes -Wno-old-style-definition -Wno-missing-prototypes'+
		' -Wno-error -Wno-cast-qual'
	@EXTRA_LINKFLAGS = ' -datasize=2000000'
	@EXTRA_EMUFLAGS = ' -allowdivzero'
	@NAME = 'newlibm-test'
end

work.invoke
