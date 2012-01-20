#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = [".", "Libs/MAUtil/automated_tests", "Libs/MAStd/automated_tests"]
	@EXTRA_INCLUDES = ["."]
	@EXTRA_CPPFLAGS = " -Wno-shadow"
	@EXTRA_LINKFLAGS = " -datasize=1024000 -stacksize=128000 -heapsize=512000"
	@LIBRARIES = ["mautil", "matest"]
	@NAME = "unitTest"
	@PACK_PARAMETERS = ' --permissions "Bluetooth,Calendar,Camera,Contacts,File Storage,Internet Access,Location,Power Management,SMS,Vibrate"'
end

work.invoke
