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
require 'ftools'

class RuntimeBuilder 
	def android(runtime_dir, mode)
		return androidBuilder(runtime_dir, mode)
	end
	
	def preprocess_android_file(src_file, src_dir, platform_dir, output_dir, platform_define)
		jtmp_file = src_file.gsub(/.jpp$/, ".jtmp")		
		java_file = src_file.gsub(/.jpp$/, ".java")
		
		puts "Processing " + java_file
		
		# Preprocess the jpp file into a jtmp file, sed fixes the output if any
		system("xgcc -x c -E -o #{output_dir}#{jtmp_file} -D#{platform_define} -I#{$SETTINGS[:java_source]}shared -I#{platform_dir}" +
		       " #{src_dir}#{src_file} 2>&1 | sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1\\2(\\3):/\"")
		
		# Use sed to comment the lines which the proprocessor added to the file and save it as a java file
		system("sed \"s/^# /\\/\\//\" < #{output_dir}#{jtmp_file} > #{output_dir}#{java_file}");
	end

	def preprocess_shared_java_files(output_dir, platform_dir, platform_define)
		shared_src = "#{$SETTINGS[:java_source]}shared/";

		Dir.foreach(shared_src) {|x| 
			if (x == "BigPhatError.jpp" || x == "Binary.jpp" || x == "BinaryInterface.jpp" || x == "Core.jpp" ||
				x == "LimitedLengthInputStream.jpp" || x == "LittleEndianDataInputStream.jpp" || x == "ThreadPool.jpp" || 
				x == "UBin.jpp" || x == "RefByteArrayOutputStream.jpp" || x == "Tilemap.jpp" ||
				x == "ImageCache.jpp" || x == "MAPanicReport.jpp" )
				preprocess_java_file(x, shared_src, platform_dir, output_dir, platform_define)
			end
		}
	end
	
	def androidBuilder(runtime_dir, mode)
		debug = (mode=="debug") ? "D" : ""
		
		android_sdk = "#{$SETTINGS[:android_sdk]}"
		
		if ENV['ANDROIDSDKDIR'] != nil
			android_sdk = ENV['ANDROIDSDKDIR']
		end
		
		android_source = "#{$SETTINGS[:java_source]}platforms/android/src"
		
		# Set up temporary dir
		temp_dir = "#{runtime_dir}temp/com/mosync/java/android/"
		if File.exist? temp_dir
			FileUtils.rm_rf temp_dir # delete everything in it and itself
		end
		File.makedirs(temp_dir); # No such directory/file.. create a temp directory
		
		# Set up class dir
		class_dir = "#{runtime_dir}class/"
		if File.exist? class_dir
			FileUtils.rm_rf class_dir # delete everything in it and itself
		end
		Dir.mkdir class_dir; # No such directory/file.. create a temp directory
		
		# Copy the old config_platform.h file and copy the one from the runtime_dir to the source location
		config_file = "#{android_source}/config_platform.h"		
		backup_file config_file
		FileUtils.copy_file( "#{runtime_dir}config#{debug}.h", config_file)
		
		# Preprocess all the shared java files and store result in temporary location
		preprocess_shared_java_files(temp_dir, android_source, "_android");
		
		# Preprocess all the platform dependant java files and store result in temporary location
		Dir.foreach(android_source) {|x| 
			if (x == "MoSync.jpp" || x == "MoSyncView.jpp" || x == "Syscall.jpp" || x == "Core.jpp")
				preprocess_java_file(x, "#{android_source}/", android_source, temp_dir, "_android")
			end
		}
		
		# Restore config_platform.h
		revert_backupped_file config_file
	
		# Build Android package file
		android_proj_root = "#{$SETTINGS[:java_source]}platforms/android/AndroidProject/"
		system("#{android_sdk}/tools/aapt package -f -v -M #{android_proj_root}/AndroidManifest.xml -F #{runtime_dir}resources.ap_ -I #{android_sdk}/android.jar -S #{android_proj_root}/res -m -J #{runtime_dir}temp");

		# Compile all the java files into class files
		system("javac -source 1.6 -target 1.6 -d #{class_dir} -classpath #{android_sdk}android.jar #{temp_dir}*.java");
	
		# Convert class files into dex files
		system("#{android_sdk}/tools/dx.bat --verbose --dex --output=#{runtime_dir}classes.dex #{class_dir} ");
		
		# Create unsigned APK and add the dex files
		system("D:/android-sdk-windows-1.5_r3/tools/apkbuilder.bat #{runtime_dir}MoSyncRuntime#{debug}.apk -u -z #{runtime_dir}resources.ap_ -f #{runtime_dir}classes.dex");
		
		# # Clean and delete all the temporary folders
		FileUtils.rm_rf "#{runtime_dir}temp"
		FileUtils.rm_rf class_dir
		FileUtils.rm runtime_dir + "classes.dex"
		FileUtils.rm runtime_dir + "resources.ap_"
		
		if !File.exist? "#{runtime_dir}MoSyncRuntime#{debug}.apk"
			puts "\nFATAL ERROR! - No apk file built, check previous output for errors!\n\n"
			return 1
		else
			puts "\nFINISHED! - #{runtime_dir}MoSyncRuntime#{debug}.apk was succesfully generated!\n\n"
			return 0
		end
		return 0
	end
	
end
