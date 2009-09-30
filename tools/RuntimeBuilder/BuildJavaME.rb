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

require 'settings'

require 'ftools'
require 'fileutils'
			
class RuntimeBuilder 
	
	def javame(runtime_dir)
		javameBuilder(runtime_dir, false);
	end
	
	def javamecldc10(runtime_dir)
		javameBuilder(runtime_dir, true);
	end
	
	def preprocessJavaFile(srcFile, srcDir, platformDir, outputDir, platformDefine)
	
#		String jtmpFile = srcFile.replaceAll(".jpp", ".jtmp");
#		System.out.println(jtmpFile);
		jtmpFile = srcFile.gsub(/.jpp$/, ".jtmp")
		
#		String javaFile = srcFile.replaceAll(".jpp", ".java");
#		System.out.println(javaFile);
		javaFile = srcFile.gsub(/.jpp$/, ".java")
		
#		exec("xgcc -x c -E -o " + outputDir + jtmpFile + " -D" + platformDefine + " -I" + javaSourceDir + "shared " +
#		    " -I" + javaSourceDir + platformDir + " " + srcDir + srcFile +
#			" 2>&1 | sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1\\2(\\3):/\"",
#			true);

#		exec("sed \"s/^# /\\/\\//\" < " + outputDir + jtmpFile + " > " + outputDir + javaFile, true);
	end

	def preprocessSharedJavaFiles(outputDir, platformDir, platformDefine)
#		String sharedSrcDir = javaSourceDir + "shared\\";
		sharedSrc = $settings[:java_source] + "\\shared";
#		File f = new File(sharedSrcDir);
		Dir.foreach("testdir") {|x| puts "Got #{x}" }
#		String files[] = f.list();
#		for (int i = 0; i < files.length; i++)
#		{
#			if (files[i].equals("BigPhatError.jpp") || files[i].equals("Binary.jpp") ||
#				files[i].equals("BinaryInterface.jpp") || files[i].equals("Core.jpp") ||
#				files[i].equals("LimitedLengthInputStream.jpp") || files[i].equals("LittleEndianDataInputStream.jpp") ||
#				files[i].equals("ThreadPool.jpp") || files[i].equals("UBin.jpp") || 
#				files[i].equals("RefByteArrayOutputStream.jpp") || files[i].equals("Tilemap.jpp") || 
#				files[i].equals("Tileset.jpp") || files[i].equals("ImageCache.jpp") || 
#				files[i].equals("MAPanicReport.jpp"))
#			{
#				preprocessJavaFile(files[i], sharedSrcDir, platformDir, outputDir, platformDefine);
#			}
#		}

	end
	
	def javameBuilder(runtime_dir, cldc10)
		javaMESource = $setting[:java_source] + "/platforms/JavaME/src"
	
		# Check if its CLCD10
		
		# Set up dirs in temporary location
		temp_dir = runtime_dir + "/temp"
		if File.exist?(temp_dir)
			if !File.directory?(temp_dir)
				File.delete temp_dir # oops, it was a file. This is strange, just delete it
			else
				FileUtils.rm_rf temp_dir # delete everything in it and itself
			end
		end
		Dir.mkdir(temp_dir); # No such directory/file.. create a temp directory
		
		# Copy the old config_platform.h file and copy the one from the runtime_dir to the source location
		configBakFile = javaMESource + "/config_platform.h.bak"
		configFile = javaMESource + "/config_platform.h"
		
		if File.exist? configBakFile
			File.delete configBakFile
		end
		if File.exist? configFile
			File.copy(configFile, configBakFile)
		end
		File.copy( runtime_dir + "config.h", configFile)
		
		# Preprocess all the shared java files and store result in temporary location
		preprocessSharedJavaFiles(temp_dir, javaMESource, "_JavaME");
		
		# Preprocess all the platform dependant java files and store result in temporary location
		
		# Restore config_platform.h
		if File.exist? configBakFile
			File.copy(configBakFile, configFile)
			File.delete configBakFile
		end	
	
		# Compile Java source
#		exec("javac -source 1.4 -target 1.4 " + " -d " + classDir + " -classpath " + classDir +
#			" -bootclasspath " +
#			javaMESdkDir + "j2melib\\jsr082.jar;" +
#			javaMESdkDir + "j2melib\\cldcapi11.jar;" +
#			javaMESdkDir + "j2melib\\midpapi20.jar;" +
#			javaMESdkDir + "j2melib\\wma20.jar;" +
#			javaMESdkDir + "j2melib\\jsr179.jar " + " " +
#			tempDir + "*.java", true);
			
		# Generate Manifest file
#		DataOutputStream dos = new DataOutputStream(new FileOutputStream(tempDir + "\\manifest.mf"));
#		writeDosCompatibleString(dos, "MIDlet-1: MoSyncRuntime" + vmInfo.number +
#			" , MoSyncRuntime" + vmInfo.number + ".png, MAMidlet\n");
#		writeDosCompatibleString(dos, "MIDlet-Name: MAMidlet\n");
#		writeDosCompatibleString(dos, "MIDlet-Vendor: Mobile Sorcery\n");
#		writeDosCompatibleString(dos, "MIDlet-Version: 1.0\n");
#		writeDosCompatibleString(dos, "MicroEdition-Configuration: CLDC-1.1\n");
#		writeDosCompatibleString(dos, "MicroEdition-Profile: MIDP-2.0\n");
#		dos.close();
		
		# Build jar file
#		exec("jar cfm " +
#		runtimeDir + "MoSyncRuntimeTemp.jar " +
#		tempDir + "manifest.mf " +
#		"-C " + classDir + " .", true);
		
		# Obfuscate java binaries
#		exec("java " +
#		"-jar " + javaMESdkDir + "bin\\proguard.jar " +
#		"-injars " + runtimeDir + "MoSyncRuntimeTemp.jar " +
#		"-libraryjars " + javaMESdkDir + "j2melib\\cldcapi11.jar " +
#		"-libraryjars " + javaMESdkDir + "j2melib\\midpapi20.jar " +
#		"-libraryjars " + javaMESdkDir + "j2melib\\jsr082.jar " +
#		"-libraryjars " + javaMESdkDir + "j2melib\\jsr179.jar " +
#		"-libraryjars " + javaMESdkDir + "j2melib\\wma20.jar " +
#		"-dontusemixedcaseclassnames " +
#		"-outjars " + runtimeDir + "MoSyncRuntimeObfuscated.jar " +
#		"-keep public class MAMidlet", true);
		
#		String D = debug ? "D" : "";
#		exec("move " + runtimeDir + "MoSyncRuntimeObfuscated.jar " + runtimeDir +
#			"MoSyncRuntime" + D + ".jar ", true);
		
		# Preverify java binaries
#		exec(javaMESdkDir + "bin\\" + "preverify " +
#			//				"-d " + preverifiedClassDir + " " +
#		"-d " + runtimeDir.substring(0, runtimeDir.length() - 1) + " " +
#		"-classpath " +
#			//				runtimeDir + "MoSyncRuntimeObfuscated.jar" +
#		javaMESdkDir + "j2melib\\jsr082.jar;" +
#		javaMESdkDir + "j2melib\\cldcapi11.jar;" +
#		javaMESdkDir + "j2melib\\midpapi20.jar;" +
#		javaMESdkDir + "j2melib\\jsr179.jar;" +
#		javaMESdkDir + "j2melib\\wma20.jar " +
#		runtimeDir + "MoSyncRuntime" + D + ".jar", true);
		
		# Clean and delete all the temporary folders
		FileUtils.rm_rf temp_dir
		
	end
	
end
