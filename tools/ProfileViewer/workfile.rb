#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ['.']
	@SPECIFIC_CFLAGS = { 'SDL_gfxPrimitives.c' => ' -Wno-unreachable-code -Wno-unused-function' }
	@LIBRARIES = ['SDL', 'SDL_ttf', 'expat']
	@NAME = 'profileViewer'
	@INSTALLDIR = mosyncdir + '/bin'
	setup
end

work.invoke
