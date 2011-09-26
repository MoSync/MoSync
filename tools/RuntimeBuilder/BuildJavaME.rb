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
require 'pathname'
require 'fileutils'

class RuntimeBuilder
	def javame(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, false)
	end

	def javamecldc10(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, true)
	end

	BB_API_ROOT = File.expand_path(File.dirname(__FILE__) + '/../ReleasePackageBuild/build_package_tools/mosync_bin')

	def bb40(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, false, ["#{BB_API_ROOT}/bb40/net_rim_api.jar"])
	end

	def bb47(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, false, ["#{BB_API_ROOT}/bb47/net_rim_api.jar"])
	end

	def bb500(runtime_dir, mode)
		return javameBuilder(runtime_dir, mode, false, ["#{BB_API_ROOT}/bb500/net_rim_api.jar"])
	end

	def preprocess_java_file(src_file, src_dir, platform_dir, output_dir, platform_define)
		jtmp_file = src_file.gsub(/.jpp$/, ".jtmp")
		java_file = src_file.gsub(/.jpp$/, ".java")

		puts "Processing " + java_file

		#
		# Preprocess the jpp file into a jtmp file, sed fixes the output if any
		# Note: The include order is very important, infact, it is critical
		#       that output_dir is included before any of the other include
		#       dirs. config_platform.h is located in output dir. That one
		#       should override any other.
		#
		sh("xgcc -x c -E -o #{output_dir}/#{jtmp_file} -D#{platform_define} -I#{output_dir} -I#{$SETTINGS[:java_source]}/Shared -I#{platform_dir}" +
		       " #{src_dir}#{src_file} 2>&1 | sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1\\2(\\3):/\"")

		# Use sed to comment the lines which the proprocessor added to the file and save it as a java file
		sh("sed \"s/^# /\\/\\//\" < #{output_dir}/#{jtmp_file} > #{output_dir}/#{java_file}");
	end

	def preprocess_shared_java_files(output_dir, platform_dir, platform_define)
		shared_src = "#{$SETTINGS[:java_source]}/Shared/";

		Dir.foreach(shared_src) {|x|
			if (x == "BigPhatError.jpp" || x == "Binary.jpp" || x == "BinaryInterface.jpp" || x == "Core.jpp" ||
				x == "LimitedLengthInputStream.jpp" || x == "LittleEndianDataInputStream.jpp" || x == "ThreadPool.jpp" ||
				x == "UBin.jpp" || x == "RefByteArrayOutputStream.jpp" || x == "ImageCache.jpp" || x == "MAPanicReport.jpp" )
				preprocess_java_file(x, shared_src, platform_dir, output_dir, platform_define)
			end
		}
	end

	def javameBuilder(runtime_dir, mode, cldc10, extra_classpath_jars = [])
		#
		# Setup
		#
		debug = (mode=="debug") ? "D" : ""
		$SETTINGS[:java_source] = Pathname::new( $SETTINGS[:java_source] ).realpath.to_s
		runtime_dir = Pathname::new( runtime_dir ).realpath.to_s

		java_me_sdk = Pathname::new( "#{$SETTINGS[:javame_sdk]}" ).realpath.to_s

		if ENV['JAVAMESDKDIR'] != nil
			java_me_sdk = ENV['JAVAMESDKDIR']
		end

		java_me_source = Pathname::new( "#{$SETTINGS[:java_source]}/platforms/javaME/src" ).realpath.to_s

		# Set up temporary dir
		temp_dir = "#{runtime_dir}/temp/"
		if File.exist? temp_dir
			FileUtils.rm_rf temp_dir # delete everything in it and itself
		end
		Dir.mkdir(temp_dir); # No such directory/file.. create a temp directory
		temp_dir = Pathname::new( temp_dir ).realpath.to_s

		# Set up class dir
		class_dir = "#{runtime_dir}/class/"
		if File.exist? class_dir
			FileUtils.rm_rf class_dir # delete everything in it and itself
		end
		Dir.mkdir class_dir; # No such directory/file.. create a temp directory
		class_dir = Pathname::new( class_dir ).realpath.to_s

		# Copy the config file to the temp dir where all the
		# files will be pre-proccessed
		config_file = "#{temp_dir}/config_platform.h"
		FileUtils.copy_file( "#{runtime_dir}/config#{debug}.h", config_file)

		# Preprocess all the shared java files and store result in temporary location
		preprocess_shared_java_files(temp_dir, java_me_source, "_JavaME");

		# Preprocess all the platform dependant java files and store result in temporary location
		Dir.foreach(java_me_source) {|x|
			if (x == "MainCanvas.jpp" || x == "MAMidlet.jpp" || x == "Syscall.jpp" || x == "Core.jpp" ||
				(x == "Real.jpp" && cldc10) || x == "SplitResourceStream.jpp")
				preprocess_java_file(x, "#{java_me_source}/", java_me_source, temp_dir, "_JavaME")
			end
		}

		cldcjar = cldc10 ? 'cldcapi10' : 'cldcapi11'
		libjars = [cldcjar, 'midpapi20', 'jsr082', 'jsr179', 'wma20', 'mmapi', 'jsr75']

		# Compile Java source
		puts "\nCompiling java source.."
		sh("javac -source 1.4 -target 1.4 -d #{class_dir} -classpath #{class_dir} -bootclasspath " +
			libjars.collect { |jar| "#{java_me_sdk}/j2melib/#{jar}.jar#{File::PATH_SEPARATOR}" }.join('') +
			extra_classpath_jars.collect { |jar|"#{jar}#{File::PATH_SEPARATOR}" }.join('') +
			" #{temp_dir}/*.java")

		# Generate Manifest file
		puts "\nGenerating manifest file.."
		runtime_number = runtime_dir.split('/')[-1] # extract which runtime number it is from the path
		File.open("#{temp_dir}/manifest.mf", "w")  do |infile|
			infile.puts "MIDlet-1: MoSyncRuntime#{runtime_number}, MoSyncRuntime#{runtime_number}.png, MAMidlet\n"
			infile.puts "MIDlet-Name: MAMidlet\n"
			infile.puts "MIDlet-Vendor: Mobile Sorcery\n"
			infile.puts "MIDlet-Version: 1.0\n"
			infile.puts "MicroEdition-Configuration: CLDC-1.#{cldc10 ? '0' : '1'}\n"
			infile.puts "MicroEdition-Profile: MIDP-2.0\n"
		end

		# Build jar file
		puts "\nBuilding jar file.."
		sh("jar cfm #{runtime_dir}/MoSyncRuntimeTemp.jar #{temp_dir}/manifest.mf -C #{class_dir} .")

		if !File.exist? "#{runtime_dir}/MoSyncRuntimeTemp.jar"
			puts "\nFATAL ERROR! - Unable to build jar file, check previous output for errors!\n\n"
			return
		end

		# Obfuscate/Preverify java binaries
		# Note: It is very important that this operation is run from a
		#       temporary directory if we are to run several builds
		#       concurrently. The reason for this is that preverify
		#       creates a temporary directory based on the current UTC
		#       time.
		#
		puts "\nProguaring java binaries.."
		curr_dir = FileUtils.pwd()
		FileUtils.cd temp_dir
		sh("java -jar #{java_me_sdk}/bin/proguard.jar -injars #{runtime_dir}/MoSyncRuntimeTemp.jar " +
			"-dontnote " +
			libjars.collect { |jar| "-libraryjars #{java_me_sdk}/j2melib/#{jar}.jar " }.join('') +
			extra_classpath_jars.collect { |jar| "-libraryjars #{jar} " }.join('') +
			" -dontusemixedcaseclassnames" +
			" -microedition" +
			" -allowaccessmodification" +
			" -outjars #{runtime_dir}/MoSyncRuntimeObfuscated.jar -keep public class MAMidlet");
		FileUtils.mv("#{runtime_dir}/MoSyncRuntimeObfuscated.jar", "#{runtime_dir}/MoSyncRuntime#{debug}.jar" )
		FileUtils.cd curr_dir

		# Clean and delete all the temporary folders
		FileUtils.rm_rf temp_dir
		FileUtils.rm_rf class_dir
		FileUtils.rm runtime_dir + "/MoSyncRuntimeTemp.jar"

		if !File.exist? "#{runtime_dir}/MoSyncRuntime#{debug}.jar"
			puts "\nFATAL ERROR! - No jar file built, check previous output for errors!\n\n"
			return 1
		else
			puts "\nFINISHED! - #{runtime_dir}/MoSyncRuntime#{debug}.jar was succesfully generated!\n\n"
			return 0
		end
		return 0
	end

end
