require "#{File.dirname(__FILE__)}/host.rb"
require "#{File.dirname(__FILE__)}/native_lib.rb"
require "#{File.dirname(__FILE__)}/exe.rb"
require "#{File.dirname(__FILE__)}/dll.rb"

module NativeMoSyncWork
	def define_cflags
		bd = File.expand_path(File.dirname(__FILE__) + "/..")
		if(HOST == :win32)
			@EXTRA_INCLUDES += [bd+"/tools/ReleasePackageBuild/build_package_tools/include"]
			custom_lib_dir = bd+"/tools/ReleasePackageBuild/build_package_tools/lib/"
			need(:@CUSTOM_LIBS)
			@CUSTOM_LIBS.each { |cl| @EXTRA_LINKFLAGS += " " + custom_lib_dir + cl }
		end
		@EXTRA_INCLUDES += [bd+"/intlibs", bd+"/libs"]
		@EXTRA_CFLAGS += " -D_POSIX_SOURCE"	#avoid silly bsd functions
		super
	end
end

class NativeMoSyncLib < NativeLibWork
	include NativeMoSyncWork
end

class MoSyncExe < ExeWork
	include NativeMoSyncWork
end

class MoSyncDll < DllWork
	include NativeMoSyncWork
end
