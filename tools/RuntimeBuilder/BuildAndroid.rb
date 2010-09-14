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

require 'fileutils'
require '../../rules/mosync_util.rb'


# The Android builder is designed so that it's using the build files which
# is located in the source tree where the runtime is located.

class RuntimeBuilder 

	# if version is not defined cupcake, 1.5, will be used
	def android(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "3")
	end
	
	def android_3(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "3")
	end
	
	def android_4(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "4")
	end
	
	def android_5(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "5")
	end
	
	def android_6(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "5")
	end
	
	def android_7(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "5")
	end
	
	def android_8(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "5")
	end

	def androidBuilder(runtime_dir, mode, version)
		debug = (mode=="debug") ? "D" : ""
		
		android_sdk = "#{$SETTINGS[:android_sdk]}/platforms/android-#{version}"
		android_ndk = "#{$SETTINGS[:android_ndk]}"
		
		cpath = FileUtils.pwd
		
		# We must fix the path here, if it's relative things will break later..
		Dir.chdir runtime_dir
		runtime_dir = FileUtils.pwd + "/"
		Dir.chdir cpath
		
		Dir.chdir "../../runtimes/java/platforms/androidJNI"
		
		puts "ruby buildJNI.rb #{android_ndk} #{android_sdk} #{runtime_dir} #{debug}"
		if (!system "ruby buildJNI.rb #{android_ndk} #{android_sdk} #{runtime_dir} #{debug}")
			puts "\nFATAL ERROR! - No android source package built, check previous output for errors!\n\n"
			return 1
		end
				
		Dir.chdir cpath
		
		if !File.exist? "#{runtime_dir}MoSyncRuntime#{debug}.zip"
			puts "\nFATAL ERROR! - No android source package built, check previous output for errors!\n\n"
			return 1
		else
			puts "\nFINISHED! - #{runtime_dir}MoSyncRuntime#{debug}.zip, Android source package was succesfully generated!\n\n"
			return 0
		end
		return 0
	end
	
end
