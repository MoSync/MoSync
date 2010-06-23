#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_SOURCEFILES = ["../pipe-tool/disas.c", "../../libs/MAUtil/RefCounted.cpp",
		"../../intlibs/helpers/TranslateSyscall.cpp", "../../intlibs/helpers/intutil.cpp"]
	@EXTRA_INCLUDES = ["../../intlibs", "../../runtimes/cpp/core","../pipe-tool",
		"../../runtimes/cpp/base", "../../runtimes/cpp/platforms/sdl"]
	if(HOST == :darwin)
		@EXTRA_INCLUDES = @EXTRA_INCLUDES + ["/sw/include"]
	end
	@LOCAL_LIBS = ["mosync_log_file", "mosync_bluetooth", "net", "stabs"]
	if(HOST == :win32)
		@CUSTOM_LIBS = ["SDL.lib", "libirprops.a", "libuuid.a"]
		@LIBRARIES = ["wsock32", "ws2_32"]
	else
		@LIBRARIES = ["SDL", "SDLmain"]
	end
	if(HOST == :linux)
		@LIBRARIES << "bluetooth"
		@EXTRA_SOURCEFILES << "../../runtimes/cpp/platforms/sdl/mutexImpl.cpp"
	end
	@SPECIFIC_CFLAGS = {
		"Value.cpp" => " -Wno-float-equal -Wno-sign-compare",
		"disas.c" => " -Wno-missing-format-attribute -Wno-unreachable-code",
	}
	@EXTRA_CFLAGS = " -DPTAH_EXPORTS -DLOGGING_ENABLED"
	@EXTRA_CPPFLAGS = " -DPTAH_EXPORTS -DLOGGING_ENABLED"
	@IGNORED_FILES = ["operations_generated.cpp"]
	@NAME = "mdb"
	@TARGETDIR = "."
	@INSTALLDIR = mosyncdir + '/bin'
end

class OpGenTask < FileTask
	def setup
		@prerequisites = [FileTask.new(@work, "operationsGen.rb"), FileTask.new(@work, "../../intlibs/stabs/types.rb")]
	end
	def execute
		sh "ruby operationsGen.rb"
	end
end

gen = OpGenTask.new(work, "operations_generated.h")
gen.setup
gen.invoke
gen = OpGenTask.new(work, "operations_generated.cpp")
gen.setup
gen.invoke

work.invoke
