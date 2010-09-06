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
		
		@IGNORED_FILES << "strptime.c"
		if (!SDL_SOUND)
			@EXTRA_CPPFLAGS += " -D__NO_SDL_SOUND__"
			@IGNORED_FILES += ["SDLSoundAudioSource.cpp"]
		end
		if(FULLSCREEN == "true")
			@EXTRA_CPPFLAGS += " -D__USE_FULLSCREEN__ -D__USE_SYSTEM_RESOLUTION__"
		end
		if(NATIVE_RUNTIME == "true")
			@IGNORED_FILES += ["PIMImpl.cpp", "pim.cpp"]
		end
		@EXTRA_INCLUDES = common_includes + ["/usr/include/gtk-2.0",
			"/usr/include/glib-2.0", "/usr/include/pango-1.0",
			"/usr/include/cairo", "/usr/include/atk-1.0",
			"/usr/lib/glib-2.0/include", "/usr/lib/gtk-2.0/include"]
		@LIBRARIES = common_libraries
		
	elsif(HOST == :darwin)
		@IGNORED_FILES << "strptime.c"
		@EXTRA_INCLUDES = common_includes + ["/sw/include", "/opt/local/include"]
		@LIBRARIES = common_libraries +["objc"]
		@SPECIFIC_CFLAGS = {"SDL_prim.c" => " -Wno-float-equal -Wno-unreachable-code -D_ANSI_SOURCE",
			"Syscall.cpp" => " -Wno-float-equal"}
	else
		error "Unsupported platform"
	end
	
	if(!@GCC_IS_V4)
		@SPECIFIC_CFLAGS["hashmap.cpp"] = " -Wno-unreachable-code"
		@SPECIFIC_CFLAGS["AudioChannel.cpp"] = " -Wno-unreachable-code"
		if(CONFIG == "")	#buggy compiler
			@SPECIFIC_CFLAGS["ConfigParser.cpp"] = " -Wno-uninitialized"
			@SPECIFIC_CFLAGS["Syscall.cpp"] = " -Wno-uninitialized -Wno-float-equal"
		end
	end
	
	@NAME = "mosync_sdl"
end

config = CopyFileTask.new(work, "config_platform.h", FileTask.new(work, "config_platform.h.example")) 
contacts = CopyFileTask.new(work, "contacts.xml", FileTask.new(work, "#{mosyncdir}/etc/contacts.xml")) 

config.invoke
work.invoke
