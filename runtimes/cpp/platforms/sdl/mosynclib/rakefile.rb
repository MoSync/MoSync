BD = "../../../../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = []
EXTRA_SOURCEFILES = ["mosynclib.cpp"]
EXTRA_INCLUDES = ["#{BD}/runtimes/cpp/base", ".."]

if ( HOST == "linux" )
	LOCAL_LIBS = ["mosync_sdl", "mosync_log_file", "mosync_bluetooth", "net", "amr"]	
else
	LOCAL_LIBS = ["mosync_sdl", "mosync_log_file", "mosync_bluetooth", "net"]
	LOCAL_DLLS = ["amr"]
end

COMMON_LIBRARIES = ["SDL", "SDL_image", "SDL_ttf"]

if(HOST == "win32") then
	CUSTOM_LIBS = COMMON_LIBRARIES.collect do |lib| "#{lib}.lib" end +
		["libexpat.lib", "SDL_sound.lib", "libirprops.a", "libuuid.a"]
	LIBRARIES = ["wsock32", "ws2_32"]
else
	if ( SDL_SOUND == false )
		SOUND_LIB = []

		if ( HOST_PLATFORM != "moblin" )
			EXTRA_CXXFLAGS = " -D__NO_SDL_SOUND__"
		else
			EXTRA_CXXFLAGS = " -D__NO_SDL_SOUND__ -D__USE_FULLSCREEN__"
		end

		IGNORED_FILES = [ "SDLSoundAudioSource.cpp" ]
	else
		SOUND_LIB = [ "SDL_sound" ]
		IGNORED_FILES = [ ]
	end

	LIBRARIES = COMMON_LIBRARIES + SOUND_LIB + ["gtk-x11-2.0", "bluetooth", "expat"]
end

DLLNAME = "mosync"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"

declare_compile_task("mosyncmain.cpp", method(:build_cpp))
MOSYNCMAIN_O = "#{BUILDDIR}mosyncmain.o"
OTARGET = "#{BD}/#{BUILDDIR}mosyncmain.o"
file OTARGET => MOSYNCMAIN_O do
	sh "cp #{MOSYNCMAIN_O} #{OTARGET}"
end
task :default => OTARGET
