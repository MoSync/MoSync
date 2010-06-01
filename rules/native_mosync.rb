# Copyright (C) 2009 Mobile Sorcery AB
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# This file defines variants of the native Work classes
# with extra includes and flags used by some of MoSync's programs.

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
