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

# buildJNI.rb <ANDROID_NDK_PATH> <ANDROID_SDK_PATH> <CONFIG_PATH> <DEBUG>


cpath = FileUtils.pwd

if ENV['MOSYNC_SRC'] == nil
	FileUtils.cd "../../../../"
	ENV['MOSYNC_SRC'] = FileUtils.pwd
	puts "MOSYNC_SRC " + FileUtils.pwd
	FileUtils.cd cpath
end

mosyncpp = "../../../cpp"
FileUtils.cd mosyncpp
ENV['MOSYNC_CPP'] = FileUtils.pwd
puts "MOSYNC_CPP " + FileUtils.pwd
FileUtils.cd cpath

mosyncppsource = "../../../cpp/platforms/android"
FileUtils.cd mosyncppsource
ENV['MOSYNC_CPP_SRC'] = FileUtils.pwd
puts "MOSYNC_CPP_SRC " + FileUtils.pwd
FileUtils.cd cpath

ENV['MOSYNC_JAVA_SRC'] = cpath

if ARGV[0] == nil
	puts "missing argument, android NDK path is unknown!"
	exit 0
end

if ARGV[1] == nil
	puts "missing argument, android SDK path is unknown!"
	exit 0
end

if ARGV[2] != nil
	# change name on the current config_platform.h to config_platform.h.saved

	conf_file = "/src/config_platform.h"
	if File.exist? conf_file
		FileUtils.copy_file "/src/config_platform.h", "/src/config_platform.h.saved"
	end

	configfile = "config.h"
	if ARGV[3] == nil
		configfile = "configD.h"
	end
	
		# copy the config.h file to it's correct position and change it's name to config_platform.h
		FileUtils.copy_file( "#{ARGV[2]}/#{configfile}", "/src/config_platform.h")
end

puts "Building native Library\n\n"

FileUtils.cd "AndroidProject"
puts FileUtils.pwd

system "#{ARGV[0]}/ndk-build"



puts "Building Java Source\n\n"

FileUtils.cd ".."
puts FileUtils.pwd

system "ruby buildJava.rb"


class_dir = "temp/"
if File.exist? class_dir
	FileUtils.rm_rf class_dir # delete everything in it and itself
end
Dir.mkdir class_dir; # No such directory/file.. create a temp directory


# Build Android package file
package_root = "#{cpath}/AndroidProject/"
system("#{ARGV[1]}../../tools/aapt package -f -v -M #{package_root}/AndroidManifest.xml -F resources.ap_ -I #{ARGV[1]}/android.jar -S #{package_root}/res -m -J #{package_root}src");
		
# Compile all the java files into class files
system("javac -source 1.6 -target 1.6 -g -d #{class_dir} -classpath #{ARGV[1]}android.jar #{package_root}/src/com/mosync/java/android/*.java #{package_root}/src/com/mosync/internal/android/*.java");

# copy the library file
FileUtils.copy_file( "#{cpath}/AndroidProject/libs/armeabi/libmosync.so", "temp/libmosync.so")

# package the files
FileUtils.cd "temp"
system "tar -zcf runtime.tar.gz libmosync.so com/"
FileUtils.copy_file( "runtime.tar.gz", "../runtime.tar.gz")
FileUtils.cd ".."

# clean up
FileUtils.rm_rf class_dir

if ARGV[2] != nil
	# change name on config_platform.h.saved to config_platform.h if such file exists
	conf_file = "/src/config_platform.h.saved"
	if File.exist? conf_file
		FileUtils.copy_file "/src/config_platform.h.saved", "/src/config_platform.h"
	end
end
