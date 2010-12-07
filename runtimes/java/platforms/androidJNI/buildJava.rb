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
# Preprocess the .jpp files and writes them to their output location were they can then be compiled
#

##
# Preprocesses the specified source file and puts it in the
# given output dir. The preprocessed file will have the extension
# .java.
#
# @param shared_dir The directory that contains the shared include files.
# @param src_file The absolute or relative path to the file to preprocess.
# @param output_dir The directory where the resulting .java file will be placed.
#
def preprocess_android_file(shared_dir, src_file, output_dir)
	jtmp_file = File.basename(src_file).gsub(/.jpp$/, ".jtmp")		
	java_file = File.basename(src_file).gsub(/.jpp$/, ".java")
	
	puts "Processing " + java_file
	
	buildFlags = "-D_android"
	
	# Add the _ANDROID_BLUETOOTH flag when building for version-5 or higher 
	androidVersion = Integer(ENV['MOSYNC_ANDROID_BLUETOOTH'])
	if androidVersion >= 7
		buildFlags << " -D_ANDROID_BLUETOOTH"
	end 
	
	# Preprocess the jpp file into a jtmp file, sed fixes the output if any
	success = system("xgcc -x c -E -o #{File.join(output_dir, jtmp_file)} #{buildFlags} -I#{shared_dir} -Isrc " + 
                   "#{src_file} 2>&1 | sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1\\2(\\3):/\"")
	
	if (!success)
		exit 1
	end
	
	# Use sed to comment the lines which the proprocessor added to the file and save it as a java file
	success = system("sed \"s/^# /\\/\\//\" < #{File.join(output_dir, jtmp_file)} > #{File.join(output_dir, java_file)}");
	
	if (!success)
		exit 1
	end
	
	FileUtils.remove File.join(output_dir, jtmp_file)
end

##
# Preprocesses a set of specified files in the given source directory.
#
# @param shared_dir The directory that contains the shared include files.
# @param source_dir The directory to search for source files.
# @param source_files A list of file paths relative to source_dir that should be preprocessed.
# @param output_dir The directory where the preprocessed files will end up.
#
def preprocess_multiple_android_files(shared_dir, source_dir, source_files, output_dir)
	source_files.each do |file|
		source_file = File.join(source_dir, file)
		if (File.exist?(source_file))
			preprocess_android_file(shared_dir, source_file, output_dir)
		end
	end
end
	
if !File.exist? "src/config_platform.h"
	puts "\nFATAL ERROR! - No config_platform.h file found!"
	return 1
end

source_dir = "src"

shared_java_source = "../../Shared"
if(ENV['MOSYNC_SRC'] != nil)
	shared_java_source = ENV['MOSYNC_SRC'] + "/runtimes/java/Shared"
end
puts "shared lib: " + shared_java_source

# Preprocess the com.mosync.java.android files, these will later be transformed to the applications real package
#
output_dir = "AndroidProject/src/com/mosync/java/android/"
source_files = ["MoSync.jpp", "MoSyncPanicDialog.jpp", "TextBox.jpp"] #, "MoSyncView.jpp", "MoSyncThread.jpp", "ThreadPool.jpp", "BigPhatError.jpp"]
preprocess_multiple_android_files(shared_java_source, source_dir, source_files, output_dir)

# Preprocess the com.mosync.internal.android files
#
output_dir = "AndroidProject/src/com/mosync/internal/android/"
source_files = ["MoSyncView.jpp" , "MoSyncThread.jpp" , "ThreadPool.jpp" , "BigPhatError.jpp"]
preprocess_multiple_android_files(shared_java_source, source_dir, source_files, output_dir)

exit 0
