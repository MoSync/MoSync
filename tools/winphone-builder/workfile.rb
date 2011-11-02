#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ['.', 'pugixml-1.0/src']
	@SPECIFIC_CFLAGS = {
		'pugixml.cpp' => ' -Wno-shadow -Wno-float-equal'
	}
	@LOCAL_LIBS = ['filelist']
	@NAME = 'winphone-builder'
	@TARGETDIR = '.'
	@INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
