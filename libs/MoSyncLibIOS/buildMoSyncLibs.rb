#!/usr/bin/ruby

require "../../rules/util.rb"
require "../../rules/mosync_util.rb"
require "FileUtils"

#FileUtils.rm_rf(["build"])


xcodebuild = "/Developer/usr/bin/xcodebuild";
if(!File.exist?(xcodebuild))
	xcodebuild = "/Applications/Xcode.app/Contents/Developer/usr/bin/xcodebuild"
end

# build everything
sh "xcodebuild -target MoSyncLibIOS -configuration Debug  -sdk iphoneos -project MoSyncLibIOS.xcodeproj"
sh "xcodebuild -target MoSyncLibIOS -configuration Release -sdk iphoneos -project MoSyncLibIOS.xcodeproj"
sh "xcodebuild -target MoSyncLibIOS -configuration Debug  -sdk iphonesimulator -project MoSyncLibIOS.xcodeproj"
sh "xcodebuild -target MoSyncLibIOS -configuration Release -sdk iphonesimulator -project MoSyncLibIOS.xcodeproj"

FileUtils.mkpath(["libs/ios_debug", "libs/ios_release"])

# concatenate libraries
sh "lipo build/Debug-iphoneos/libMoSyncLibIOS.a build/Debug-iphonesimulator/libMoSyncLibIOS.a -create -output libs/ios_debug/MoSyncLibs.a"
sh "lipo build/Release-iphoneos/libMoSyncLibIOS.a build/Release-iphonesimulator/libMoSyncLibIOS.a -create -output libs/ios_release/MoSyncLibs.a"

FileUtils.mkpath([mosyncdir+"/lib", mosyncdir+"/include/MAStdNative"]);

FileUtils.cp_r Dir.glob('libs/*'), mosyncdir+"/lib", :verbose => true
FileUtils.cp_r Dir.glob('MAStdNative/*'), mosyncdir+"/include/MAStdNative", :verbose => true
