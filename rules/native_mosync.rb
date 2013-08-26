# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This file defines variants of the native Work classes
# with extra includes and flags used by some of MoSync's programs.

require "#{File.dirname(__FILE__)}/host.rb"
require "#{File.dirname(__FILE__)}/native_lib.rb"
require "#{File.dirname(__FILE__)}/exe.rb"
require "#{File.dirname(__FILE__)}/dll.rb"
require "#{File.dirname(__FILE__)}/mosync_util.rb"

module NativeMoSyncWork
	def define_cflags
		bd = File.expand_path_fix(File.dirname(__FILE__) + "/..")
		if(HOST == :win32)
			@EXTRA_INCLUDES += [bd+"/tools/ReleasePackageBuild/build_package_tools/include"]
			custom_lib_dir = bd+"/tools/ReleasePackageBuild/build_package_tools/lib/"
			need(:@CUSTOM_LIBS)
			@CUSTOM_LIBS.each { |cl| @EXTRA_LINKFLAGS += " " + custom_lib_dir + cl }
		elsif(HOST == :darwin) 
			@EXTRA_INCLUDES += ["/sw/include", "/opt/local/include"]	
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
