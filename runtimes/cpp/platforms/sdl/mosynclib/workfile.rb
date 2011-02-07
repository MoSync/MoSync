#!/usr/bin/ruby

require File.expand_path('../shared_work.rb')

# todo: make sure only syscalls are exported.

work = MoSyncDll.new
class << work
	include SdlCommon
end

work.instance_eval do
	setup_common
	
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["main.cpp"]
	@SPECIFIC_CFLAGS = {"main.cpp" => " -DMOSYNC_DLL_EXPORT -Wno-missing-noreturn",
		"mosyncmain.cpp" => " -DMOSYNC_DLL_IMPORT"}
	@WHOLE_LIBS = ["mosync_sdl"]
	if(HOST == :win32)
		@LIBRARIES += ["OpenGL32", "GlU32", "Gdi32"]
		@EXTRA_OBJECTS = [FileTask.new(self, "mosynclib.def")]
	end
	
	@NAME = "mosync"
end

main = NativeMoSyncLib.new
main.instance_eval do
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["mosyncmain.cpp"]
	@NAME = "mosyncmain"
end

work.invoke
main.invoke
