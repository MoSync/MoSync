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

	def javaSource
		@javaSource = "../../runtimes/java";
	end
	
	def javame(runtime_dir)
		javameBuilder(runtime_dir, false);
	end
	
	def javamecldc10(runtime_dir)
		javameBuilder(runtime_dir, true);
	end
	
	def preprocessJavaFile(srcFile, srcDir, platformDir, outputDir, platformDefine)
#		String jtmpFile = srcFile.replaceAll(".jpp", ".jtmp");
#		System.out.println(jtmpFile);
#		String javaFile = srcFile.replaceAll(".jpp", ".java");
#		System.out.println(javaFile);
#		exec("xgcc -x c -E -o " + outputDir + jtmpFile + " -D" + platformDefine + " -I" + javaSourceDir + "shared " +
#		    " -I" + javaSourceDir + platformDir + " " + srcDir + srcFile +
#			" 2>&1 | sed \"s/\\([a-zA-Z/]\\+\\)\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1\\2(\\3):/\"",
#			true);
#		exec("sed \"s/^# /\\/\\//\" < " + outputDir + jtmpFile + " > " + outputDir + javaFile, true);

	end
	
	def javameBuilder(runtime_dir, cldc10)
		javaMESource = javaSource.concat "/platforms/JavaME/src"
	
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
		File.copy(configFile, configBakFile)
		File.copy( runtime_dir + "config.h", configFile)
		
		# Preprocess all the shared java files and store result in temporary location
		
		# Preprocess all the platform dependant java files and store result in temporary location
		
		# Restore config_platform.h
		File.copy(configBakFile, configFile)
		File.delete configBakFile
	
		# Compile Java source
		
		# Generate Manifest file
		
		# Build jar file
		
		# Obfuscate java binaries
		
		# Preverify java binaries
		
		# Clean and delete all the temporary folders
		FileUtils.rm_rf temp_dir
		
	end
	
end

rbuild = RuntimeBuilder.new
rbuild.javame("D:/MoSync/profiles/runtimes/java/1");
#puts Dir.entries("..//.")
