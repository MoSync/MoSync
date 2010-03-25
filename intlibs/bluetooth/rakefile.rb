BD = "../.."
require "#{BD}/build_rules/host.rb"

EXTRA_INCLUDES = ["#{BD}/runtimes/cpp/base", "#{BD}/runtimes/cpp/platforms/sdl"]

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
LIBNAME = "mosync_bluetooth"

if ( HOST == "linux" )
	SOURCES = [".", "linux", "linux/bluez"]
	if ( BLUETOOTH == true )
		EXTRA_CXXFLAGS = " -DBLUEZ_SUPPORTED"
	end
end

if(HOST == "win32")
	SOURCES = [".", "win32"]
end

#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------
require "#{BD}/build_rules/native.rb"


SOURCE_OBJECTS.each do |o| task o => "config_bluetooth.h" end

file "config_bluetooth.h" => "config_bluetooth.h.example" do |t|
	sh "cp #{t.prerequisites} #{t.name}"
end
