#!/usr/bin/ruby

require File.expand_path('../shared_work.rb')
require File.expand_path('../../../../../rules/mosync_util.rb')

work = MoSyncExe.new
class << work
	include SdlCommon
end
work.instance_eval do
	setup_common
	
	BD = '../../../../..'
	@SOURCES = ["."]
	@IGNORED_FILES = ["debugger.cpp"]
	@EXTRA_SOURCEFILES = ["#{BD}/runtimes/cpp/core/Core.cpp",
		"#{BD}/runtimes/cpp/core/sld.cpp",
		"#{BD}/runtimes/cpp/core/GdbStub.cpp",
		"#{BD}/runtimes/cpp/core/extensions.cpp",
		"#{BD}/intlibs/helpers/intutil.cpp",
		]
	@EXTRA_INCLUDES += ["../../.."]
	@SPECIFIC_CFLAGS = { "Core.cpp" => " -DHAVE_IOCTL_ELLIPSIS" }
	if(!@GCC_IS_V4 && CONFIG=="debug")
		@SPECIFIC_CFLAGS["Core.cpp"] += " -Wno-unreachable-code"
		@SPECIFIC_CFLAGS["sld.cpp"] = " -Wno-unreachable-code"
	end
	if(HOST == :win32)
		@EXTRA_LINKFLAGS = ' -mwindows'
	end
	
	@LOCAL_LIBS = ["mosync_sdl", "demangle"] + @LOCAL_LIBS
	
	@NAME = "MoRE"
	
	@INSTALLDIR = mosyncdir + '/bin'
	
	setup
end

work.invoke
