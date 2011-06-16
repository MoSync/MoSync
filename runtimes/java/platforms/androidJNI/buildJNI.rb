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

def exitBuilder(arg, configDir, config)
	if config != nil
		# change name on config_platform.h.saved to config_platform.h if such file exists
		conf_file = File.join(configDir, "config_platform.h.saved")
		if File.exist? conf_file
			FileUtils.copy_file conf_file, File.join(configDir, "config_platform.h")
		end
	end
	exit Integer(arg)
end

cpath = pwd

sh "ruby addLibraries.rb"

androidNDKPath = ARGV[0]
androidSDKPath = ARGV[1]
configPath = ARGV[2]
debugFlag = ARGV[3]

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

if androidNDKPath == nil
	puts "missing argument, android NDK path is unknown!"
	exit 1
end

if androidSDKPath == nil
	puts "missing argument, android SDK path is unknown!"
	exit 1
end

debug = (debugFlag == nil) ? "" : "D"

outdir = ".."
if configPath != nil
	outdir = configPath
	
	# change name on the current config_platform.h to config_platform.h.saved

	conf_file = File.join(mosyncppsource, "config_platform.h")
	if File.exist? conf_file
		puts "saving config file"
		FileUtils.copy_file conf_file, File.join(mosyncppsource, "config_platform.h.saved")
	end

	runtime_config = File.join(configPath, "config#{debug}.h")
	puts "using runtime #{runtime_config}"
	
	# copy the config.h file to it's correct position and change it's name to config_platform.h
	FileUtils.copy_file( runtime_config, conf_file)
end


puts "Building native Library\n\n"

cd "AndroidProject"

if ENV['OS'] == "Windows_NT"
	# convert a copy of cygwin.sh to unix-style line endings, so bash can run it.
	FileUtils.copy_file("#{cpath}/cygwin.sh", "#{cpath}/cygwin_u.sh")
	if(File.exist?("/cygwin/bin/bash.exe"))
		cygPath = "/cygwin/bin/"
	elsif(nil != ENV["CYGPATH"])
		cygPath = ENV["CYGPATH"]
	elsif(system("bash.exe pwd"))
		cygPath = ""
	else
		msg = "Can not find the cygwin installation.\n" +
			  "It is either not installed or cannot be found in the default location"+
			  "or it is not included in PATH.\n"+
			  " If it is installed try adding your cygwin bin directory to either" +
			  "PATH or CYGPATH environmental variable"
		raise Exception.new(msg)
	end
	success = system "#{cygPath}bash.exe --login -c \"dos2unix $(cygpath -u #{cpath}/cygwin_u.sh)\""
	if (!success)
		exitBuilder(1, mosyncppsource, configPath)
	end

	success = system "#{cygPath}bash.exe --login -i #{File.join(cpath, "cygwin_u.sh")} #{androidNDKPath} #{androidSDKPath} #{ENV['MOSYNC_SRC']}"
else
	success = system("#{File.join(cpath, "invoke-ndk-build.sh")} #{androidNDKPath} #{androidSDKPath} $MOSYNC_SRC");
end

if (!success)
	exitBuilder(1, mosyncppsource, configPath)
end

puts "Getting into to Android Java runtime root directory."
cd ".."
puts pwd

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

if(!File.exist?("#{package_root}/gen"))
	mkdir("#{package_root}/gen")
end

success = system(
	"#{File.join(androidSDKPath, "tools/aapt")} package -f -v " +
	"-M #{File.join(package_root,"AndroidManifest.xml")} -F resources.ap_ " +
	"-I #{File.join(androidSDKPath, "android.jar")} " +
	"-S #{File.join(package_root, "res")} " +
	"-m -J #{File.join(package_root, "gen")}");
	
puts "Compiling Java Source Files\n\n"

packages = ["src/com/mosync/java/android/*.java",
            "src/com/mosync/internal/android/*.java",
            "src/com/mosync/internal/generated/*.java",
            "src/com/mosync/nativeui/core/*.java",
			"src/com/mosync/nativeui/ui/egl/*.java",
            "src/com/mosync/nativeui/ui/factories/*.java",
            "src/com/mosync/nativeui/ui/widgets/*.java",
            "src/com/mosync/nativeui/util/*.java",
            "src/com/mosync/nativeui/util/properties/*.java"
            ]

# Concatenate each list element with package_root, and flatten the list to a string
java_files = packages.map { |package| File.join(package_root, package) }.join(" ")

# Compile all the java files into class files
sh(
	"javac -source 1.6 -target 1.6 -g -d #{class_dir} " +
	"-classpath " +
	"#{File.join(androidSDKPath, "android.jar")} " + java_files)

if (!success)
	exitBuilder(1, mosyncppsource, configPath)
end

puts "Copy Generated Library File\n\n"

# copy the library file
FileUtils.copy_file( "#{File.join(cpath, "AndroidProject/libs/armeabi/libmosync.so")}", "temp/libmosync.so")

puts "Build Zip Package\n\n"

# package the files
cd "temp"

if ENV['OS'] == "Windows_NT"
	sh("#{ENV['MOSYNC_SRC']}/tools/ReleasePackageBuild/build_package_tools/mosync_bin/zip -r MoSyncRuntime#{debug}.zip .");
else
	success = system("zip -r MoSyncRuntime#{debug}.zip .");
end
if (!success)
	exitBuilder(1, mosyncppsource, configPath)
end

FileUtils.copy_file( "MoSyncRuntime#{debug}.zip", File.join(outdir, "MoSyncRuntime#{debug}.zip"))

cd ".."

# Delete temp dir.
FileUtils.rm_rf class_dir

if (!success)
	exitBuilder(0, mosyncppsource, configPath)
end

