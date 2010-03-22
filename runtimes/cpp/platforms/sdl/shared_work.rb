require File.expand_path('../../../../../rules/native_mosync.rb')

module SdlCommon
def setup_common
	@EXTRA_INCLUDES = ["../../../base", ".."]
	
	@LOCAL_LIBS = ["mosync_log_file", "mosync_bluetooth", "net", "filelist", "amr"]
	common_libraries = ["SDL", "SDL_image", "SDL_ttf"]
	
	if(HOST == :win32) then
		@CUSTOM_LIBS = common_libraries.collect do |lib| "#{lib}.lib" end +
			["libexpat.lib", "SDL_sound.lib", "libirprops.a", "libuuid.a"]
		@LIBRARIES = ["wsock32", "ws2_32"]
	elsif(HOST == :linux) then
		@EXTRA_CPPFLAGS = ""
		@IGNORED_FILES = []
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
		
		@LIBRARIES = common_libraries + sound_lib + ["gtk-x11-2.0", "bluetooth", "expat"]
	else
		error "Unsupported platform"
	end
end
end
