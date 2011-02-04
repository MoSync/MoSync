#!/usr/bin/ruby

require File.expand_path('../../../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CPPFLAGS = ""
	@EXTRA_INCLUDES =["/sw/include", "/opt/local/include"]
	@LIBRARIES = ["SDL", "SDLmain", "SDL_net"]
	@NAME = "nativeuidebug-server"
	#@INSTALLDIR = "build/."
end

work.invoke
