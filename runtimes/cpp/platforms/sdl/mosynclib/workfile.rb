require File.expand_path('../../../../../rules/native_mosync.rb')

# todo: make sure only syscalls are exported.

work = MoSyncDll.new
work.instance_eval do
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["mosynclib.cpp"]
	@EXTRA_INCLUDES = ["../../../base", ".."]
	
	@LOCAL_LIBS = ["mosync_sdl", "mosync_log_file", "mosync_bluetooth", "net"]
	@LOCAL_DLLS = ["amr"]
	common_libraries = ["SDL", "SDL_image", "SDL_ttf"]
	
	if(HOST == :win32) then
		#puts "yo"
		@CUSTOM_LIBS = common_libraries.collect do |lib| "#{lib}.lib" end +
			["libexpat.lib", "SDL_sound.lib", "libirprops.a", "libuuid.a"]
		#puts @CUSTOM_LIBS
		@LIBRARIES = ["wsock32", "ws2_32"]
	elsif(HOST == :linux) then
		if(SDL_SOUND)
			sound_lib = [ "SDL_sound" ]
		else
			sound_lib = []
			@EXTRA_CPPFLAGS = " -D__NO_SDL_SOUND__"
			@IGNORED_FILES += [ "SDLSoundAudioSource.cpp" ]
		end
		
		if(HOST_PLATFORM == :moblin)
			@EXTRA_CPPFLAGS += " -D__USE_FULLSCREEN__"
		end
		
		@LIBRARIES = common_libraries + sound_lib + ["gtk-x11-2.0", "bluetooth", "expat"]
	else
		error "Unsupported platform"
	end
	
	@NAME = "mosync"
	
	setup
	
	MAIN = CompileGccTask.new(self, @COMMON_BUILDDIR + "mosyncmain.o", FileTask.new(self, "mosyncmain.cpp"), "")
end

work.invoke
MAIN.invoke
