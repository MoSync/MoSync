# Copyright (C) 2010 MoSync AB
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

#
# Helper function that preprocesses the .jpp files and writes them to 
# their output location were they can then be compiled.
#
def preprocess_android_file(src_file, src_dir, shared_dir, output_dir)
	jtmp_file = src_file.gsub(/.jpp$/, ".jtmp")		
	java_file = src_file.gsub(/.jpp$/, ".java")
	
	puts "Processing " + java_file
	
	buildFlags = "-D_android"
	
	# Get the api level 
	androidVersion = Integer(ENV['MOSYNC_ANDROID_API_LEVEL'])
	
	# Add the _ANDROID_BLUETOOTH flag when building for version-5 or higher.
	if androidVersion >= 7
		buildFlags << " -D_ANDROID_BLUETOOTH"
		buildFlags << " -D_ANDROID_API_LEVEL_7_OR_HIGHER"
	end 
	
	if androidVersion >= 5
		buildFlags << " -D_ANDROID_API_LEVEL_5_OR_HIGHER"
	end 
	
	if androidVersion == 3
		buildFlags << " -D_ANDROID_API_LEVEL_3"
	end 
	
	# Preprocess the jpp file into a jtmp file, sed fixes the output if any.
	success = system(
		"xgcc -x c -E -o #{output_dir}#{jtmp_file} #{buildFlags} " +
		"-I#{shared_dir} -Isrc #{src_dir}#{src_file} 2>&1 | " +
		"sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):" +
		"\\([0-9]\\+\\):/\\1\\2(\\3):/\"")
	
	if (!success)
		exit 1
	end
	
	# Use sed to comment the lines which the proprocessor 
	# added to the file and save it as a java file.
	success = system(
		"sed \"s/^# /\\/\\//\" < #{output_dir}#{jtmp_file} > " +
		"#{output_dir}#{java_file}");
	
	if (!success)
		exit 1
	end
	
	FileUtils.remove "#{output_dir}#{jtmp_file}"
end

#
# Script starts doing processing from here.
#

if !File.exist? "src/config_platform.h"
	puts "\nFATAL ERROR! - No config_platform.h file found!"
	return 1
end

android_source = "src"

shared_java_source = "../../Shared"
if(ENV['MOSYNC_SRC'] != nil)
	shared_java_source = ENV['MOSYNC_SRC'] + "/runtimes/java/Shared"
end
puts "shared lib: " + shared_java_source

out_dir = "AndroidProject/src/com/mosync/java/android/"

#
# Preprocess com.mosync.java.android files, these will later be 
# renamed to the applications real package name.
#
Dir.foreach(android_source) {|x| 
	if (x == "MoSync.jpp" || 
		x == "MoSyncPanicDialog.jpp" || 
		x == "TextBox.jpp" || 
		x == "MoSyncService.jpp" || 
		x == "MoSyncAutoStart.jpp")
		preprocess_android_file(
			x, 
			"#{android_source}/", 
			shared_java_source, out_dir)
	end
}

out_dir = "AndroidProject/src/com/mosync/internal/android/"

#
# Preprocess com.mosync.internal.android files.
#
Dir.foreach(android_source) {|x| 
	if (x == "MoSyncView.jpp" || 
		x == "MoSyncThread.jpp" || 
		x == "ThreadPool.jpp" || 
		x == "BigPhatError.jpp" )
		preprocess_android_file(
			x, 
			"#{android_source}/", 
			shared_java_source, out_dir)
	end
}

exit 0
