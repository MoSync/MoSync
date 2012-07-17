#
# This file is used to combine our JavaScript libraries and
# add them to templates and MoSync apckage
require "fileutils"

# Specify the JavaScript files to be
# included in wormhole.js here.
FilesIncludedInWormholeJS = [
  "mosync-bridge.js",
  "phonegap-bridge.js",
  "phonegap-1.2.0.js",
  "mosync-resource.js",
  "mosync-nativeui.js",
  "mosync-sensormanager.js",
  "mosync-pushnotifications.js"
]

# Specify directories to copy wormhole.js to.
DirectoriesToCopyWormholeJSTo = [
  "../../../templates/HTML5 JS C++ Hybrid Project/",
  "../../../templates/HTML5 JS NativeUI Project/",
  "../../../templates/HTML5 JS WebUI Project/",
  "../../../examples/html5/WormholeDemo/LocalFiles/js/",
  "../../../examples/html5/WormholeNativeUI/LocalFiles/js/",
  "../../../testPrograms/PhoneGapMoSync/LocalFiles/js/",
  "../../../testPrograms/phonegap/WormholePushNotificationTest/LocalFiles/js/"
]

# Specify directories to copy mosync-bridge.js to.
DirectoriesToCopyMoSyncBridgeTo = [
  "../../../examples/html5/WebViewLoveSMS/LocalFiles/js/",
  "../../../examples/html5/WebViewTwitter/LocalFiles/js/",
  "../../../examples/html5/NFCExample/LocalFiles/js/",
  "../../../testPrograms/WormholeBenchmark/LocalFiles/js/"
]

# Divider between files included in wormhole.js
Divider = "\n// =============================================================\n//\n"

def copyWormholeJS
  dirList = DirectoriesToCopyWormholeJSTo
  dirList.each do |dirName|
    if(!File.exist?(dirName))
      FileUtils.mkdir_p(dirName)
    end
    FileUtils.cp("wormhole.js", dirName)
  end
end

def copyMoSyncBridge
  dirList = DirectoriesToCopyMoSyncBridgeTo
  dirList.each do |dirName|
    FileUtils.cp("mosync-bridge.js", dirName)
  end
end

def copyJSFilesToMoSync
  fileList = FilesIncludedInWormholeJS
  fileList << "wormhole.js"

  # Get the Mosync directory path
  mosyncDir = ENV["MOSYNCDIR"]

  # Make sure target dir in MoSync dir exists
  if (!File.exist?("#{mosyncDir}/jslib"))
    FileUtils.mkdir("#{mosyncDir}/jslib")
  end

  # Copy files to the MoSync dir
  fileList.each { |fileName|
    FileUtils.cp(fileName, "#{mosyncDir}/jslib/")
  }
end

def buildWormholeJS
  fileList = FilesIncludedInWormholeJS

  # Create wormhole.js by concatenating all files.
  wormholejs = ""
  fileList.each do |fileName|
    File.open(fileName, "r") do |sourceFile|
      fileContent = sourceFile.read
      wormholejs = wormholejs + Divider + "// File: " + fileName + "\n\n" + fileContent
    end
  end

  # Write wormhole.js
  File.open("wormhole.js", "w") do |targetFile|
    targetFile.write(wormholejs)
  end
end

def main
  if (ARGV.include? "build")
    buildWormholeJS
  elsif (ARGV.include? "copy")
    copyJSFilesToMoSync
    copyWormholeJS
  elsif (ARGV.include? "copybridge")
    copyMoSyncBridge
  elsif (ARGV.include? "all")
    buildWormholeJS
    copyJSFilesToMoSync
    copyWormholeJS
  else
    puts "Usage:"
    puts "  buildJS build  Builds wormhole.js"
    puts "  buildJS copy   Copies wormhole.js"
    puts "  buildJS all    Builds + Copies wormhole.js"
    puts "  buildJS copybridge    Special for copying mosync-bridge.js to target directories"
  end
end

# Call main function
main
