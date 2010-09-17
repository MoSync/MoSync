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

# usage: buildJNI.rb <ANDROID_NDK_PATH> <ANDROID_SDK_PATH> <CONFIG_PATH> <DEBUG>

# <ANDROID_NDK_PATH> 	: The path to where the ndk is located i.e. C:/Android/android-ndk-r4
# <ANDROID_SDK_PATH> 	: The path to where the sdk and the used platform is located i.e. C:/Android/android-sdk-windows/platforms/android-3 for cupcake 1.5
# <CONFIG_PATH>			: The path to where the config.h is located. If this is set the finished runtime will end up in this folder as well, other wise it will be in the project source root
# <DEBUG>				: If this is set to anything they will use the configD.h file which is supposed to be at the <CONFIG_PATH>

def exitBuilder(arg, config)
	if config != nil
		# change name on config_platform.h.saved to config_platform.h if such file exists
		conf_file = "/src/config_platform.h.saved"
		if File.exist? conf_file
			FileUtils.copy_file "/src/config_platform.h.saved", "/src/config_platform.h"
		end
	end
	exit Integer(arg)
end

cpath = FileUtils.pwd

firstarg = ARGV[0]
secondarg = ARGV[1]
thirdarg = ARGV[2]
fortharg = ARGV[3]

if ENV['MOSYNC_SRC'] == nil
	FileUtils.cd "../../../../"
	ENV['MOSYNC_SRC'] = FileUtils.pwd
	FileUtils.cd cpath
end

mosyncpp = "../../../cpp"
FileUtils.cd mosyncpp
ENV['MOSYNC_CPP'] = FileUtils.pwd
FileUtils.cd cpath

mosyncppsource = "../../../cpp/platforms/android"
FileUtils.cd mosyncppsource
ENV['MOSYNC_CPP_SRC'] = FileUtils.pwd
FileUtils.cd cpath

ENV['MOSYNC_JAVA_SRC'] = cpath

if firstarg == nil
	puts "missing argument, android NDK path is unknown!"
	exit 1
end

if secondarg == nil
	puts "missing argument, android SDK path is unknown!"
	exit 1
end

# Store the current android version in the environment variable MOSYNC_ANDROID_BLUETOOTH
ENV['MOSYNC_ANDROID_BLUETOOTH'] = secondarg[-1, 1]

debug = (fortharg == nil) ? "" : "D"

outdir = ".."
if thirdarg != nil
	outdir = thirdarg
	
	# change name on the current config_platform.h to config_platform.h.saved

	conf_file = "src/config_platform.h"
	if File.exist? conf_file
		puts "saving config file"
		FileUtils.copy_file "src/config_platform.h", "src/config_platform.h.saved"
	end

	puts "using runtime #{thirdarg}config#{debug}.h"
	
	# copy the config.h file to it's correct position and change it's name to config_platform.h
	FileUtils.copy_file( "#{thirdarg}config#{debug}.h", "src/config_platform.h")
end


puts "Building native Library\n\n"

FileUtils.cd "AndroidProject"

if ENV['OS'] == "Windows_NT"
	success = system "/cygwin/bin/bash.exe --login -i #{cpath}/cygwin.sh #{firstarg} #{secondarg} #{ENV['MOSYNC_SRC']}"
else
	success = system("#{cpath}/invoke-ndk-build.sh #{firstarg} #{secondarg} $MOSYNC_SRC");
end

if (!success)
	exitBuilder(1, thirdarg)
end

puts "Preprocess Java Source Files\n\n"

FileUtils.cd ".."
puts FileUtils.pwd

success = system "ruby buildJava.rb"
if (!success)
	exitBuilder(1, thirdarg)
end

class_dir = "temp/"
if File.exist? class_dir
	FileUtils.rm_rf class_dir # delete everything in it and itself
end
Dir.mkdir class_dir; # No such directory/file.. create a temp directory

puts "Build Android package\n\n"

# Build Android package file
package_root = "#{cpath}/AndroidProject/"
system("#{secondarg}../../tools/aapt package -f -v -M #{package_root}/AndroidManifest.xml -F resources.ap_ -I #{secondarg}/android.jar -S #{package_root}/res -m -J #{package_root}src");
	
puts "Compile Java Source Files\n\n"

# Compile all the java files into class files
success = system("javac -source 1.6 -target 1.6 -g -d #{class_dir} -classpath #{secondarg}/android.jar #{package_root}/src/com/mosync/java/android/*.java #{package_root}/src/com/mosync/internal/android/*.java");
if (!success)
	exitBuilder(1,thirdarg)
end

puts "Copy Generated Library File\n\n"

# copy the library file
FileUtils.copy_file( "#{cpath}/AndroidProject/libs/armeabi/libmosync.so", "temp/libmosync.so")

puts "Build Zip Package\n\n"

# package the files
FileUtils.cd "temp"

if ENV['OS'] == "Windows_NT"
	success = system("#{ENV['MOSYNC_SRC']}/tools/ReleasePackageBuild/build_package_tools/mosync_bin/zip -r MoSyncRuntime#{debug}.zip .");
else
	success = system("zip -r MoSyncRuntime#{debug}.zip .");
end
if (!success)
	exitBuilder(1, thirdarg)
end

FileUtils.copy_file( "MoSyncRuntime#{debug}.zip", "#{outdir}/MoSyncRuntime#{debug}.zip")

FileUtils.cd ".."

# clean up
FileUtils.rm_rf class_dir

if (!success)
	exitBuilder(0, thirdarg)
end

