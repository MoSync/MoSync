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

	# If version is not defined, 2.3.3 (API level 10) will be used.
	# In fact we currently use API level 10 for all runtime versions.
	# but we need to pass the version to the other script so it knows
	# which version it builds for
	def android(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "10")
	end

	def android_3(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "3")
	end

	def android_4(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "4")
	end

	def android_5(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "5")
	end

	def android_6(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "6")
	end

	def android_7(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "7")
	end

	def android_7x(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "7")
	end

	def android_8(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "8")
	end

	def android_9(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "9")
	end

	def android_10(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode, "10", "10")
	end

	def androidBuilder(runtime_dir, mode, platform, version)
		debug = (mode=="debug") ? "D" : ""
		android_tools = "#{$SETTINGS[:android_sdk]}/platform-tools"
		android_sdk = "#{$SETTINGS[:android_sdk]}/platforms/android-#{platform}"
		android_ndk = "#{$SETTINGS[:android_ndk]}"

		cpath = FileUtils.pwd

		# We must fix the path here, if it's relative things will break later..
		Dir.chdir runtime_dir
		runtime_dir = FileUtils.pwd + "/"
		Dir.chdir cpath

		Dir.chdir "../../runtimes/java/platforms/androidJNI"

		sh("ruby buildJNI.rb #{android_ndk} #{android_sdk} #{android_tools} #{runtime_dir} #{version} #{debug}")

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
