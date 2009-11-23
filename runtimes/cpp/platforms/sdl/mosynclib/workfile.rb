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
end

main = NativeMoSyncLib.new
main.instance_eval do
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["mosyncmain.cpp"]
	@NAME = "mosyncmain"
end

work.invoke
main.invoke
