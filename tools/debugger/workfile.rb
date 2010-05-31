#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_SOURCEFILES = ["../pipe-tool/disas.c", "../../libs/MAUtil/RefCounted.cpp", "../../intlibs/helpers/TranslateSyscall.cpp", "../../intlibs/helpers/intutil.cpp"]
	@EXTRA_INCLUDES = ["../../intlibs", "../../runtimes/cpp/core","../pipe-tool"]
	if(HOST == :darwin)
		@EXTRA_INCLUDES = @EXTRA_INCLUDES + ["/sw/include"]
	end
	@LOCAL_LIBS = ["mosync_log_file", "mosync_bluetooth", "net", "stabs"]
	@LIBRARIES = ["SDL", "SDLmain"]
	@SPECIFIC_CFLAGS = { "expression.cpp" => " -Wno-shadow",
"async.cpp" => " -Wno-shadow", "Value.cpp" => " -Wno-float-equal -Wno-sign-compare"}
	@EXTRA_CFLAGS = " -DPTAH_EXPORTS -DLOGGING_ENABLED -Wno-strict-prototypes -Wno-missing-prototypes -Wno-missing-format-attribute -Wno-missing-noreturn -Wno-shadow -Wno-unreachable-code -Wno-write-strings -Wno-multichar -fno-strict-aliasing"
	@EXTRA_CPPFLAGS = " -DPTAH_EXPORTS -DLOGGING_ENABLED -Wno-missing-format-attribute -Wno-missing-noreturn -Wno-shadow -Wno-unreachable-code -Wno-unused-variable"
	@IGNORED_FILES = ["operations_generated.cpp"]
	@NAME = "mdb"
	@TARGETDIR = "."
end

work.invoke
