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

require "androidPaths.rb"
require 'fileutils'

def preprocess_android_file(src_file, src_dir, shared_dir, output_dir)
	jtmp_file = src_file.gsub(/.jpp$/, ".jtmp")		
	java_file = src_file.gsub(/.jpp$/, ".java")
	
	puts "Processing " + java_file
	
	# Preprocess the jpp file into a jtmp file, sed fixes the output if any
	system("xgcc -x c -E -o #{output_dir}#{jtmp_file} -D_android -I#{shared_dir} -Isrc" +
		" #{src_dir}#{src_file} 2>&1 | sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1\\2(\\3):/\"")
	
	# Use sed to comment the lines which the proprocessor added to the file and save it as a java file
	system("sed \"s/^# /\\/\\//\" < #{output_dir}#{jtmp_file} > #{output_dir}#{java_file}");
	
	FileUtils.remove "#{output_dir}#{jtmp_file}"
end
	
if !File.exist? "src/config_platform.h"
	puts "\nFATAL ERROR! - No config_platform.h file found!"
	return 1
end

android_source = "src"
shared_java_source = "../../Shared"

out_dir = "AndroidProject/src/com/mosync/java/android/"

# Preprocess all the jpp files and create java files from them
Dir.foreach(android_source) {|x| 
	if (x == "MoSync.jpp" || x == "MoSyncView.jpp" || x == "MoSyncThread.jpp" || x == "ThreadPool.jpp" || x == "BigPhatError.jpp" )
		preprocess_android_file(x, "#{android_source}/", shared_java_source, out_dir)
	end
}
