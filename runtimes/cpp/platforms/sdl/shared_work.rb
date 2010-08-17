require File.expand_path('../../../../../rules/native_mosync.rb')

module SdlCommon
def setup_common
	common_includes = ["../../../base", ".."]
	
	@LOCAL_LIBS = ["mosync_log_file", "mosync_bluetooth", "net", "filelist"]
	common_libraries = ["SDL", "SDL_image", "SDL_ttf"]
	
	if(HOST == :win32) then
		@CUSTOM_LIBS = common_libraries.collect do |lib| "#{lib}.lib" end +
			["libexpat.lib", "SDL_sound.lib", "libirprops.a", "libuuid.a", "FreeImage.lib"]
			# We should also add: "WebKit.lib", "WebKitGUID.lib"
			# But we need to build versions that are not Visual Studio specific.
		@LIBRARIES = ["wsock32", "ws2_32"]
		@LOCAL_DLLS = ["amr"]
		@EXTRA_INCLUDES = ["../../../base", ".."]
	elsif(HOST == :linux) then
		@EXTRA_CPPFLAGS = ""
		@IGNORED_FILES = []
		@LOCAL_LIBS << "amr"
		if(SDL_SOUND)
			sound_lib = [ "SDL_sound" ]
		else
			sound_lib = []
			@EXTRA_CPPFLAGS += " -D__NO_SDL_SOUND__"
			@IGNORED_FILES += [ "SDLSoundAudioSource.cpp" ]
		end
		
		if(FULLSCREEN == "true")
			@EXTRA_CPPFLAGS += " -D__USE_FULLSCREEN__"
		end
		@LIBRARIES = common_libraries + sound_lib + ["gtk-x11-2.0", "bluetooth", "expat", "freeimage"]
		@EXTRA_INCLUDES = ["../../../base", ".."]
	
	elsif(HOST == :darwin)
		@LOCAL_LIBS << "amr"
		@LIBRARIES = common_libraries + ["SDL_sound", "SDLmain", "expat", "freeimage"]
		@EXTRA_INCLUDES = common_includes + ["/sw/include", "/opt/local/include"]
	else
		error "Unsupported platform"
	end
end
end
