require File.expand_path('../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do
	if(HOST == :linux)
		@SOURCES = [".", "linux", "linux/bluez"]
		if(BLUETOOTH)
			@EXTRA_CFLAGS = " -DBLUEZ_SUPPORTED"
		end
	elsif(HOST == :win32)
		@SOURCES = [".", "win32"]
		if(!GCC_IS_V4)
			@SPECIFIC_CFLAGS = { "discImpl.cpp", " -Wno-unreachable-code" }
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
