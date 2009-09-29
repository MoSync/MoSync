BD = "../../../../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = []
EXTRA_SOURCEFILES = ["mosynclib.cpp", "mosyncmain.cpp"]
EXTRA_INCLUDES = ["#{BD}/runtimes/cpp/base", ".."]

LOCAL_LIBS = ["mosync_sdl", "mosync_log_file", "mosync_bluetooth", "amr", "net"]
COMMON_LIBRARIES = ["SDL", "SDL_image", "SDL_ttf"]


if ( HOST == "win32" ) then
	CUSTOM_LIBS = (COMMON_LIBRARIES + ["SDL_sound"]).collect do |t| "#{t}.lib" end
	WHOLE_CUSTOM_LIBS = ["libirprops.a", "libuuid.a"]
	LIBRARIES =["wsock32", "ws2_32"]
	EXTRA_DLLFLAGS = " -u maWait -u maIoctl"
else
	if ( SDL_SOUND == false )
		SOUND_LIB = []
	else
		SOUND_LIB = [ "SDL_sound" ]
	end

	if ( BLUETOOTH == false )	
		LIBRARIES = COMMON_LIBRARIES + SOUND_LIB
	else
		LIBRARIES = COMMON_LIBRARIES + SOUND_LIB + ["bluetooth"]
	end

	EXTRA_CFLAGS = " -fvisibility=hidden"
	EXTRA_DLLFLAGS = " -u maWait -u maIoctl " + %x{pkg-config --libs gtk+-2.0}.chop
end

DLLNAME = "mosync"

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"
