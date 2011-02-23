#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do
	set_defaults
	
	if(HOST == :linux)
	
		@SOURCES = [".", "linux", "linux/bluez"]
		if(BLUETOOTH)
			@EXTRA_CPPFLAGS = " -DBLUEZ_SUPPORTED"
		else
			#error "libbluetooth-dev missing!"
		end	
		
		
		@EXTRA_INCLUDES = ["../../runtimes/cpp/base", "../../runtimes/cpp/platforms/sdl"]
		@SPECIFIC_CFLAGS = { "interface.cpp" => " -Wno-missing-noreturn", "discovery.cpp" => " -Wno-missing-noreturn" }
	elsif(HOST == :win32)
		@SOURCES = [".", "win32"]
		if(!@GCC_IS_V4)
			@SPECIFIC_CFLAGS = { "discImpl.cpp" => " -Wno-unreachable-code" }
		end
	elsif(HOST == :darwin)
		@SOURCES = [".", "darwin"]
		@EXTRA_CPPFLAGS = " -DCOCOA_SUPPORTED"
		@EXTRA_INCLUDES = ["../../runtimes/cpp/base", "../../runtimes/cpp/platforms/sdl"]
		@SPECIFIC_CFLAGS = { "interface.mm" => " -Wno-missing-noreturn","discovery.cpp" => " -Wno-missing-noreturn" }
	else
		error "Unknown platform: #{HOST}"
	end
	
	@NAME = "mosync_bluetooth"
	
	setup
	
	config_file = CopyFileTask.new(self, "config_bluetooth.h", FileTask.new(self, "config_bluetooth.h.example"))
	
	@source_objects.each {|so| so.prerequisites << config_file }
end

work.invoke
