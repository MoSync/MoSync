#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do
	if(HOST == :linux)
		@SOURCES = [".", "linux", "linux/bluez"]
		if(BLUETOOTH)
			@EXTRA_CPPFLAGS = " -DBLUEZ_SUPPORTED"
		else
			error "libbluetooth-dev missing!"
		end
		@EXTRA_INCLUDES = ["../../runtimes/cpp/base", "../../runtimes/cpp/platforms/sdl"]
	elsif(HOST == :win32)
		@SOURCES = [".", "win32"]
		if(!NATIVE_GCC_IS_V4)
			@SPECIFIC_CFLAGS = { "discImpl.cpp" => " -Wno-unreachable-code" }
		end
	else
		error "Unknown platform: #{HOST}"
	end
	
	@NAME = "mosync_bluetooth"
	
	setup
	
	config_file = CopyFileTask.new(self, "config_bluetooth.h", FileTask.new(self, "config_bluetooth.h.example"))
	
	@source_objects.each {|so| so.prerequisites << config_file }
end

work.invoke
