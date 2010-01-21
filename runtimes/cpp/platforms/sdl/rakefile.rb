BD = "../../../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = [".", "./thread", "../../base", "../../base/thread", "./Skinning"]
COMMON_IGNORED_FILES = ["Image.cpp", "audio.cpp"]
COMMON_INCLUDES = [".", "#{BD}/runtimes/cpp/base"]
COMMON_LIBRARIES = ["SDL", "SDLmain", "SDL_ttf"]
SPECIFIC_CFLAGS = {"SDL_prim.c" => " -Wno-float-equal",
	"Syscall.cpp" => " -Wno-float-equal"}

if(HOST == "win32") then
	EXTRA_INCLUDES = COMMON_INCLUDES
	LIBRARIES = COMMON_LIBRARIES
	IGNORED_FILES  = COMMON_IGNORED_FILES
elsif(HOST == "linux")
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

	EXTRA_INCLUDES = COMMON_INCLUDES + ["/usr/include/gtk-2.0",
		"/usr/include/glib-2.0", "/usr/include/pango-1.0",
		"/usr/include/cairo", "/usr/include/atk-1.0",
		"/usr/lib/glib-2.0/include", "/usr/lib/gtk-2.0/include"]
	LIBRARIES = COMMON_LIBRARIES + SOUND_LIB + ["gtk-x11-2.0", "bluetooth", "expat"]
end

LIBNAME = "mosync_sdl"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"

SOURCE_OBJECTS.each do |o| task o => "config_platform.h" end

file "config_platform.h" => "config_platform.h.example" do |t|
	sh "cp #{t.prerequisites} #{t.name}"
end
