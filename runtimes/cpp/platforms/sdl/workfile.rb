require File.expand_path('../../../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do 
	@SOURCES = [".", "./thread", "../../base", "../../base/thread"]
	@IGNORED_FILES = ["Image.cpp", "audio.cpp"]
	COMMON_INCLUDES = [".", "../../base"]
	COMMON_LIBRARIES = ["SDL", "SDLmain", "SDL_ttf"]
	@SPECIFIC_CFLAGS = {"SDL_prim.c" => " -Wno-float-equal -Wno-unreachable-code",
		"Syscall.cpp" => " -Wno-float-equal"}
	@EXTRA_CPPFLAGS = " -DMOSYNC_DLL_EXPORT"
	
	if(HOST == :win32) then
		@EXTRA_INCLUDES = COMMON_INCLUDES
		@LIBRARIES = COMMON_LIBRARIES
	elsif(HOST == :linux)
		
		if ( SDL_SOUND == false )
			if ( HOST_PLATFORM != :moblin )
				@EXTRA_CPPFLAGS = " -D__NO_SDL_SOUND__ "
			else
				@EXTRA_CPPFLAGS = " -D__NO_SDL_SOUND__ -D__USE_FULLSCREEN__ "
			end
			@IGNORED_FILES += [ "SDLSoundAudioSource.cpp" ]
		end
		
		@EXTRA_INCLUDES = COMMON_INCLUDES + ["/usr/include/gtk-2.0",
			"/usr/include/glib-2.0", "/usr/include/pango-1.0",
			"/usr/include/cairo", "/usr/include/atk-1.0",
			"/usr/lib/glib-2.0/include", "/usr/lib/gtk-2.0/include"]
		@LIBRARIES = COMMON_LIBRARIES
	else
		error "Unsupported platform"
	end
	
	if(!GCC_IS_V4)
		@SPECIFIC_CFLAGS["AudioChannel.cpp"] = " -Wno-unreachable-code"
	end
	
	@NAME = "mosync_sdl"
end

config = CopyFileTask.new(work, "config_platform.h", FileTask.new(work, "config_platform.h.example")) 

config.invoke
work.invoke
