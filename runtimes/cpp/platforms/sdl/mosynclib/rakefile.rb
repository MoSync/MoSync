BD = "../../../../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = []
COMMON_IGNORED_FILES = []
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
	IGNORED_FILES  = COMMON_IGNORED_FILES
else
	if ( SDL_SOUND == false )
		SOUND_LIB = []
        SDL_SOUND_FLAG   = " -D__NO_SDL_SOUND__ "
		SDL_SOUND_IGNORE = [ "SDLSoundAudioSource.cpp" ]
	else
		SOUND_LIB = [ "SDL_sound" ]
        SDL_SOUND_FLAG   = ""
		SDL_SOUND_IGNORE = [ ]
	end

    if ( FULLSCREEN == "false" )
        FULLSCR_FLAG = ""
    else
        FULLSCR_FLAG = " -D__USE_FULLSCREEN__"
    end

    EXTRA_CXXFLAGS = SDL_SOUND_FLAG + FULLSCR_FLAG
    IGNORED_FILES  = COMMON_IGNORED_FILES+SDL_SOUND_IGNORE
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
