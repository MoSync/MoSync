require "../../../../rules/util.rb"
require "FileUtils"

#FileUtils.rm_rf(["build"])

# build everything
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphoneos -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphoneos -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphonesimulator -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphonesimulator -project MoSync.xcodeproj"

# build directory structure
FileUtils.rm_rf(["libs"])
FileUtils.mkpath(["libs/debug", "libs/release"])

# concatenate libraries
sh "lipo build/Debug-iphoneos/libMoSyncLib.a build/Debug-iphonesimulator/libMoSyncLib.a -create -output libs/debug/libMoSyncLib.a"
sh "lipo build/Release-iphoneos/libMoSyncLib.a build/Release-iphonesimulator/libMoSyncLib.a -create -output libs/release/libMoSyncLib.a"

