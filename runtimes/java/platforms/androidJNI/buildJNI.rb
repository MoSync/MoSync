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
require '../../../../rules/util.rb'

include FileUtils::Verbose

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
			copy_file "/src/config_platform.h.saved", "/src/config_platform.h"
		end
	end
	exit Integer(arg)
end

cpath = pwd

sh "ruby addLibraries.rb"

firstarg = ARGV[0]
secondarg = ARGV[1]
thirdarg = ARGV[2]
fortharg = ARGV[3]

if ENV['MOSYNC_SRC'] == nil
	cd "../../../../"
	ENV['MOSYNC_SRC'] = pwd
	cd cpath
end

mosyncpp = "../../../cpp"
cd mosyncpp
ENV['MOSYNC_CPP'] = pwd
cd cpath

mosyncppsource = "../../../cpp/platforms/android"
cd mosyncppsource
ENV['MOSYNC_CPP_SRC'] = pwd
cd cpath

ENV['MOSYNC_JAVA_SRC'] = cpath

if firstarg == nil
	puts "missing argument, android NDK path is unknown!"
	exit 1
end

if secondarg == nil
	puts "missing argument, android SDK path is unknown!"
	exit 1
end

# TODO: MOSYNC_ANDROID_API_LEVEL is not used I guess! Remove it.
# Store the API level in the environment variable MOSYNC_ANDROID_API_LEVEL
#ENV['MOSYNC_ANDROID_API_LEVEL'] = secondarg[-1, 1]

debug = (fortharg == nil) ? "" : "D"

outdir = ".."
if thirdarg != nil
	outdir = thirdarg
	
	# change name on the current config_platform.h to config_platform.h.saved

	conf_file = "src/config_platform.h"
	if File.exist? conf_file
		puts "saving config file"
		copy_file "src/config_platform.h", "src/config_platform.h.saved"
	end

	puts "using runtime #{thirdarg}config#{debug}.h"
	
	# copy the config.h file to it's correct position and change it's name to config_platform.h
	copy_file( "#{thirdarg}config#{debug}.h", "src/config_platform.h")
end


puts "Building native Library\n\n"

cd "AndroidProject"

if ENV['OS'] == "Windows_NT"

	sh "/cygwin/bin/bash.exe --login -c \"dos2unix $(cygpath -u #{cpath}/cygwin.sh)\""

	sh "/cygwin/bin/bash.exe --login -i #{cpath}/cygwin.sh #{firstarg} #{secondarg} #{ENV['MOSYNC_SRC']}"
else
	sh("#{cpath}/invoke-ndk-build.sh #{firstarg} #{secondarg} $MOSYNC_SRC");
end

# TODO: Delete commented out code when we are sure it is not needed.
##puts "Preprocess Java Source Files\n\n"

# Go to Android Java runtime root directory.
cd ".."
puts pwd

# TODO: Delete commented out code when we are sure it is not needed.
# Not used, there are no longer any .jpp files to build.
#sh "ruby buildJava.rb"

# Create temporary directory used for output.
# First make sure delete it if it exists to make 
# sure we get an empty directory.
class_dir = "temp/"
if File.exist? class_dir
	rm_rf class_dir # delete everything in it and itself
end
Dir.mkdir class_dir; # No such directory/file.. create a temp directory

# Don't build Android package file; it'll be done later, by the packager.
package_root = "#{cpath}/AndroidProject/"
sh(
	"#{secondarg}/tools/aapt package -f -v -M " +
	"#{package_root}/AndroidManifest.xml -F resources.ap_ -I " +
	"#{secondarg}/android.jar -S " +
	"#{package_root}/res -m -J " +
	"#{package_root}src")

puts "Compile Java Source Files\n\n"

# Compile all the java files into class files
sh(
	"javac -source 1.6 -target 1.6 -g -d #{class_dir} " +
	"-classpath " +
	"#{secondarg}/android.jar " +
	"#{package_root}/src/com/mosync/java/android/*.java " +
	"#{package_root}/src/com/mosync/internal/android/*.java " +
	"#{package_root}/src/com/mosync/internal/generated/*.java")


puts "Copy Generated Library File\n\n"

# copy the library file
copy_file( "#{cpath}/AndroidProject/libs/armeabi/libmosync.so", "temp/libmosync.so")

puts "Build Zip Package\n\n"

# package the files
cd "temp"

if ENV['OS'] == "Windows_NT"
	sh("#{ENV['MOSYNC_SRC']}/tools/ReleasePackageBuild/build_package_tools/mosync_bin/zip -r MoSyncRuntime#{debug}.zip .");
else
	sh("zip -r MoSyncRuntime#{debug}.zip .");
end

copy_file( "MoSyncRuntime#{debug}.zip", "#{outdir}/MoSyncRuntime#{debug}.zip")

cd ".."

# Delete temp dir.
rm_rf class_dir
