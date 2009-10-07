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

require 'ftools'
require 'fileutils'
			
class RuntimeBuilder 
	def javame(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, false)
	end
	
	def javamecldc10(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, true)
	end
	
	def preprocess_java_file(src_file, src_dir, platform_dir, output_dir, platform_define)
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
				x == "UBin.jpp" || x == "RefByteArrayOutputStream.jpp" || x == "Tilemap.jpp" || x == "Tileset.jpp" ||
				x == "ImageCache.jpp" || x == "MAPanicReport.jpp" )
				preprocess_java_file(x, shared_src, platform_dir, output_dir, platform_define)
			end
		}
	end
	
	def javameBuilder(runtime_dir, mode, cldc10)
		debug = (mode=="debug") ? "D" : ""
		
		if ENV['JAVAMESDKDIR'] == nil
			puts "Environment variable JAVAMESDKDIR not set with JavaME SDK path.. unable to build!"
			return 1
		end
		
		java_me_source = "#{$SETTINGS[:java_source]}platforms/JavaME/src"
		
		# Set up temporary dir
		temp_dir = "#{runtime_dir}temp/"
		if File.exist? temp_dir
			if !File.directory? temp_dir
				File.delete temp_dir # oops, it was a file. This is strange, just delete it
			else
				FileUtils.rm_rf temp_dir # delete everything in it and itself
			end
		end
		Dir.mkdir(temp_dir); # No such directory/file.. create a temp directory
		
		# Set up class dir
		class_dir = "#{runtime_dir}class/"
		if File.exist? class_dir
			if !File.directory? class_dir
				File.delete class_dir # oops, it was a file. This is strange, just delete it
			else
				FileUtils.rm_rf class_dir # delete everything in it and itself
			end
		end
		Dir.mkdir class_dir; # No such directory/file.. create a temp directory
		
		# Copy the old config_platform.h file and copy the one from the runtime_dir to the source location
		config_file = "#{java_me_source}/config_platform.h"		
		backup_file config_file
		File.copy( "#{runtime_dir}config#{debug}.h", config_file)
		
		# Preprocess all the shared java files and store result in temporary location
		preprocess_shared_java_files(temp_dir, java_me_source, "_JavaME");
		
		# Preprocess all the platform dependant java files and store result in temporary location
		Dir.foreach(java_me_source) {|x| 
			if (x == "MainCanvas.jpp" || x == "MAMidlet.jpp" || x == "Syscall.jpp" || x == "Core.jpp" ||
				(x == "Real.jpp" && cldc10))
				preprocess_java_file(x, "#{java_me_source}/", java_me_source, temp_dir, "_JavaME")
			end
		}
		
		# Restore config_platform.h
		revert_backupped_file config_file
	
		java_me_sdk = ENV['JAVAMESDKDIR']
	
		# Compile Java source
		puts "Compiling java source.."
		system("javac -source 1.4 -target 1.4 -d #{class_dir} -classpath #{class_dir} -bootclasspath " +
			   "#{java_me_sdk}j2melib/jsr082.jar;#{java_me_sdk}j2melib/cldcapi11.jar;" +
			   "#{java_me_sdk}j2melib/midpapi20.jar;#{java_me_sdk}j2melib/wma20.jar;" +
			   "#{java_me_sdk}j2melib/jsr179.jar #{temp_dir}*.java");
			
		# Generate Manifest file
		puts "Generating manifest file.."
		runtime_number = runtime_dir.split('/')[-1] # extract which runtime number it is from the path
		File.open("#{temp_dir}manifest.mf", "w")  do |infile|
			infile.puts "MIDlet-1: MoSyncRuntime#{runtime_number} , MoSyncRuntime#{runtime_number}.png, MAMidlet\n"
			infile.puts "MIDlet-Name: MAMidlet\n"
			infile.puts "MIDlet-Vendor: Mobile Sorcery\n"
			infile.puts "MIDlet-Version: 1.0\n"
			infile.puts "MicroEdition-Configuration: CLDC-1.1\n"
			infile.puts "MicroEdition-Profile: MIDP-2.0\n"
		end
		
		# Build jar file
		puts "Building jar file.."
		dos_class_dir = class_dir.gsub(/\//, "\\") # fixes the slashes so that the jar file gets it right
		system("jar cfm #{runtime_dir}MoSyncRuntimeTemp.jar #{temp_dir}manifest.mf -C #{dos_class_dir} .")
		
		if !File.exist? "#{runtime_dir}MoSyncRuntimeTemp.jar"
			puts "\nFATAL ERROR! - Unable to build jar file, check previous output for errors!\n\n"
			return
		end
		
		# Obfuscate java binaries
		puts "Obfuscating java binaries.."
		system("java -jar #{java_me_sdk}bin/proguard.jar -injars #{runtime_dir}MoSyncRuntimeTemp.jar " +
		"-libraryjars #{java_me_sdk}j2melib/cldcapi11.jar -libraryjars #{java_me_sdk}j2melib/midpapi20.jar " +
		"-libraryjars #{java_me_sdk}j2melib/jsr082.jar -libraryjars #{java_me_sdk}j2melib/jsr179.jar " +
		"-libraryjars #{java_me_sdk}j2melib/wma20.jar -dontusemixedcaseclassnames " +
		"-outjars #{runtime_dir}MoSyncRuntimeObfuscated.jar -keep public class MAMidlet");
		
		File.copy("#{runtime_dir}MoSyncRuntimeObfuscated.jar", "#{runtime_dir}MoSyncRuntime#{debug}.jar" )
		
		# Preverify java binaries
		puts "Preverifying java binaries.."
		system("#{java_me_sdk}bin/preverify -d #{runtime_dir[0..-2]} -classpath " +
			   "#{java_me_sdk}j2melib/jsr082.jar;#{java_me_sdk}j2melib/cldcapi11.jar;" +
			   "#{java_me_sdk}j2melib/midpapi20.jar;#{java_me_sdk}j2melib/jsr179.jar;" +
			   "#{java_me_sdk}j2melib/wma20.jar #{runtime_dir}MoSyncRuntime#{debug}.jar")
		
		# Clean and delete all the temporary folders
		FileUtils.rm_rf temp_dir
		FileUtils.rm_rf class_dir
		File.delete runtime_dir + "MoSyncRuntimeObfuscated.jar"
		File.delete runtime_dir + "MoSyncRuntimeTemp.jar"
		
		if !File.exist? "#{runtime_dir}MoSyncRuntime#{debug}.jar"
			puts "\nFATAL ERROR! - No jar file built, check previous output for errors!\n\n"
			return 1
		else
			puts "\nFINISHED! - #{runtime_dir}MoSyncRuntime#{debug}.jar was succesfully generated!\n\n"
			return 0
		end
		return 0
	end
	
end
