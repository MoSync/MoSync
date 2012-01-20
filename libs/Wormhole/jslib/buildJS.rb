#
# This file is used to combine our JavaScript libraries and
# add them to templates and MoSync apckage
require "fileutils"

# Add or rename the JavaScript Files Here
fileList = [
  "mosync-bridge.js",
  "mosync-resource.js",
  "phonegap-bridge.js",
  "mosync-nativeui.js",
  "phonegap-1.2.0.js",
  "mosync-sensormanager.js",
  "mosync-pushnotifications.js"
  ]

# Get the Mosync directory Path
mosyncDir = ENV["MOSYNCDIR"]

# Copy original files for reference
if(!File.exist?("#{mosyncDir}/jslib"))
  FileUtils.mkdir("#{mosyncDir}/jslib")
end
fileList.each { |fileName|
  FileUtils.cp(fileName, "#{mosyncDir}/jslib/")
}

# Write JavaScript Templates
# Use the same notations for new templates
File.open(
  "../../../templates/Wormhole NativeUI Project/wormhole-nativeui.js",
  "w") do |targetFile|

  fileList.each do |filename|
    File.open(filename, "r") do |sourceFile|
      fileContent = sourceFile.read;
      targetFile.write(fileContent);
    end
  end
end
FileUtils.cp(
  "../../../templates/Wormhole NativeUI Project/wormhole-nativeui.js",
  "#{mosyncDir}/jslib/")

File.open(
  "../../../templates/Wormhole Web Project/wormhole-webapp.js",
  "w") do |targetFile|

  fileList.each do |filename|
    if((!filename.include?("nativeui")) &&
      (!filename.include?("resource")))

      File.open(filename, "r") do |sourceFile|
        fileContent = sourceFile.read;
        targetFile.write(fileContent);
      end

    end
  end
end
FileUtils.cp(
  "../../../templates/Wormhole Web Project/wormhole-webapp.js",
  "#{mosyncDir}/jslib/")
