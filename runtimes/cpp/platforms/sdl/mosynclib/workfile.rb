require File.expand_path('../shared_work.rb')

# todo: make sure only syscalls are exported.

work = MoSyncDll.new
class << work
	include SdlCommon
end
work.instance_eval do
	setup_common
	
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["mosynclib.cpp"]
	@SPECIFIC_CFLAGS = {"mosynclib.cpp" => " -DMOSYNC_DLL_EXPORT",
		"mosyncmain.cpp" => " -DMOSYNC_DLL_IMPORT"}
	
	#@EXTRA_OBJECTS = [FileTask.new(self, "mosynclib.def")]
	#@EXTRA_LINKFLAGS = " mosynclib.def"
	
	@NAME = "mosync"
	
	setup
	
	source = FileTask.new(self, "mosyncmain.cpp")
	MAIN = CompileGccTask.new(self, @COMMON_BUILDDIR + "mosyncmain.o", source, getGccFlags(source))
end

work.invoke
MAIN.invoke
