require 'fileutils'
require '../../../../rules/util.rb'

include FileUtils::Verbose

androidSDKPath = ARGV[0]
cpath = pwd

# Don't build Android package file; it'll be done later, by the packager.
package_root = "#{cpath}/AndroidProject/"

if(!File.exist?("#{package_root}/gen"))
	mkdir("#{package_root}/gen")

sh("#{File.join(androidSDKPath, "tools/aapt")} package -f -v " +
	"-M #{File.join(package_root,"AndroidManifest.xml")} -F resources.ap_ " +
	"-I #{File.join(androidSDKPath, "android.jar")} " +
	"-S #{File.join(package_root, "res")} " +
	"-m -J #{File.join(package_root, "gen")}");
end

puts "Compiling Java Source Files\n\n"

class_dir = "temp/"
if File.exist? class_dir
	rm_rf class_dir # delete everything in it and itself
end
Dir.mkdir class_dir; # No such directory/file.. create a temp directory

packages = [
	"src/com/mosync/java/android/*.java",
	"src/com/mosync/internal/android/*.java",
	"src/com/mosync/internal/generated/*.java",
	"src/com/mosync/nativeui/core/*.java",
	"src/com/mosync/nativeui/ui/egl/*.java",
	"src/com/mosync/nativeui/ui/factories/*.java",
	"src/com/mosync/nativeui/ui/widgets/*.java",
	"src/com/mosync/nativeui/util/*.java",
	"src/com/mosync/nativeui/util/properties/*.java",
	"src/com/mosync/pim/*.java",
	"src/com/mosync/nativeui/ui/custom/*.java",
	"gen/com/mosync/java/android/*.java",
]

# Concatenate each list element with package_root, and flatten the list to a string
java_files = packages.map { |package| File.join(package_root, package) }.join(" ")

# Compile all the java files into class files
sh(
	"javac "+
	#"-Xlint:deprecation "+
	"-source 1.6 -target 1.6 -g -d #{class_dir} " +
	"-classpath " +
	"#{File.join(androidSDKPath, "android.jar")} " + java_files)
