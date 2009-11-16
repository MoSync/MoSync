require File.expand_path('../shared_work.rb')

work = MoSyncExe.new
class << work
	include SdlCommon
end
work.instance_eval do
	setup_common
	
	BD = "../../../../.."
	@SOURCES = ["."]
	@IGNORED_FILES = ["debugger.cpp"]
	@EXTRA_SOURCEFILES = ["#{BD}/runtimes/cpp/core/Core.cpp",
		"#{BD}/runtimes/cpp/core/sld.cpp",
		"#{BD}/runtimes/cpp/core/GdbStub.cpp",
		"#{BD}/intlibs/helpers/intutil.cpp"]
	@EXTRA_INCLUDES += ["../../.."]
	#@EXTRA_CPPFLAGS = " -save-temps"
	@SPECIFIC_CFLAGS = { "Core.cpp" => " -Wno-unreachable-code" }
	
	@NAME = "moemu"
	
	setup
end

work.invoke
