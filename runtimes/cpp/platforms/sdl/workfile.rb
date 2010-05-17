#!/usr/bin/ruby

require File.expand_path('../../../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do 
	@SOURCES = [".", "./thread", "./Skinning", "../../base", "../../base/thread", "../../../../intlibs/hashmap"]
	@IGNORED_FILES = ["Image.cpp", "audio.cpp"]
	common_includes = [".", "../../base"]
	common_libraries = ["SDL", "SDLmain", "SDL_ttf"]
	@SPECIFIC_CFLAGS = {"SDL_prim.c" => " -Wno-float-equal -Wno-unreachable-code",
		"Syscall.cpp" => " -Wno-float-equal"}
	@EXTRA_CPPFLAGS = " -DMOSYNC_DLL_EXPORT"
	
	if(HOST == :win32) then
		@EXTRA_INCLUDES = common_includes
		@LIBRARIES = common_libraries
	elsif(HOST == :linux)
		
		@EXTRA_CPPFLAGS = ""
		if (!SDL_SOUND)
			@EXTRA_CPPFLAGS += " -D__NO_SDL_SOUND__"
			@IGNORED_FILES += ["SDLSoundAudioSource.cpp"]
		end
		if(FULLSCREEN == "true")
			@EXTRA_CPPFLAGS += " -D__USE_FULLSCREEN__ -D__USE_SYSTEM_RESOLUTION__"
		end
		
		@EXTRA_INCLUDES = common_includes + ["/sw/include/gtk-2.0",
			"/sw/include/glib-2.0", "/sw/include/pango-1.0",
			"/sw/include/cairo", "/sw/include/atk-1.0",
			"/sw/lib/glib-2.0/include", "/sw/lib/gtk-2.0/include", "/sw/include"]
		@LIBRARIES = common_libraries
	else
		error "Unsupported platform"
	end
	
	if(!@GCC_IS_V4)
		@SPECIFIC_CFLAGS["AudioChannel.cpp"] = " -Wno-unreachable-code"
		if(CONFIG == "")	#buggy compiler
			@SPECIFIC_CFLAGS["ConfigParser.cpp"] = " -Wno-uninitialized"
			@SPECIFIC_CFLAGS["Syscall.cpp"] = " -Wno-uninitialized -Wno-float-equal"
		end
	end
	
	@NAME = "mosync_sdl"
end

config = CopyFileTask.new(work, "config_platform.h", FileTask.new(work, "config_platform.h.example")) 

config.invoke
work.invoke
