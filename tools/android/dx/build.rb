require 'fileutils'

# Remove any old files
if ( File.exists?( "dx.jar" ) )
	FileUtils.remove_entry "dx.jar"
end

if ( File.exists?( "bin/com" ) )
	FileUtils.remove_entry "bin/com"
end

if ( File.exists?( "bin/junit" ) )
	FileUtils.remove_entry "bin/junit"
end


# Change to bin directory
FileUtils.cd "bin"

#
# Compile
#
puts( "Compiling source files..." )
system( "javac -source 1.5 -target 1.5 -cp ../src -d . ../src/com/android/dx/command/Main.java" )
if ( $? != 0 )
	puts( "Failed!" )
	exit( 1 )
else
	puts( "OK" )
end

#
# Build jar
#
puts( "\nBuilding dx.jar ..." )
system( "jar -cm META-INF/MANIFEST.MF com junit > ../dx.jar" )
if ( $? != 0 )
	puts( "Failed!" )
	exit( 1 )
else
	puts( "OK" )
end

# Change back to main directory
FileUtils.cd "../"

puts "\nDone!"
exit( 0 )