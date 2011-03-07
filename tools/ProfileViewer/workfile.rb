#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ['.']
	@EXTRA_CFLAGS = ' -D__STRICT_ANSI__'
	@SPECIFIC_CFLAGS = { 'SDL_gfxPrimitives.c' => ' -Wno-unreachable-code -Wno-unused-function' }
	common_libraries = ['SDL', 'SDL_ttf']
	
	if(HOST == :win32) then
		@CUSTOM_LIBS = common_libraries.collect do |lib| "#{lib}.lib" end +
			['libexpat.lib']
	else	# unix
		@LIBRARIES = common_libraries + ['expat']
	end
	@NAME = 'profileViewer'
	@INSTALLDIR = mosyncdir + '/bin'
	setup
end

work.invoke
