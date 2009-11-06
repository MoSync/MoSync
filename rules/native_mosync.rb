require "#{File.dirname(__FILE__)}/host.rb"
require "#{File.dirname(__FILE__)}/native_lib.rb"
require "#{File.dirname(__FILE__)}/exe.rb"

module NativeMoSyncWork
	def define_cflags
		bd = File.expand_path(File.dirname(__FILE__) + "/..")
		if(HOST == :win32)
			@EXTRA_INCLUDES += [bd+"/tools/ReleasePackageBuild/build_package_tools/include"]
		end
		@EXTRA_INCLUDES += [bd+"/intlibs", bd+"/libs"]
			# + ((defined?(MOSYNC) == nil) ? [] : ["#{BD}/libs/MAStd"])
		super
	end
end

class NativeMoSyncLib < NativeLibWork
	include NativeMoSyncWork
end

class MoSyncExe < ExeWork
	include NativeMoSyncWork
end
