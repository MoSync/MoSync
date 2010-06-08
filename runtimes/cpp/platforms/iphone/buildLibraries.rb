require "../../../../rules/util.rb"
require "FileUtils"

#FileUtils.rm_rf(["build"])

# build everything
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphoneos -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphoneos -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphonesimulator -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphonesimulator -project MoSync.xcodeproj"

# build directory structure
FileUtils.rm_rf(["template"])
FileUtils.mkpath(["template/libs/debug", "template/libs/release", "template/Classes", "template/Classes/helpers"])

# concatenate libraries
sh "lipo build/Debug-iphoneos/libMoSyncLib.a build/Debug-iphonesimulator/libMoSyncLib.a -create -output template/libs/debug/libMoSyncLib.a"
sh "lipo build/Release-iphoneos/libMoSyncLib.a build/Release-iphonesimulator/libMoSyncLib.a -create -output template/libs/release/libMoSyncLib.a"

# these files should probably be combined into one
FileUtils.cp "Classes/maapi_defs.h", "template/Classes/maapi_defs.h"
FileUtils.cp "Classes/converters.h", "template/Classes/converters.h"
FileUtils.cp "Classes/mstypeinfo.h", "template/Classes/mstypeinfo.h"

# these are needed
FileUtils.cp "Classes/MoSyncAppDelegate.h", "template/Classes/MoSyncAppDelegate.h"
FileUtils.cp "Classes/MoSyncAppDelegate.mm", "template/Classes/MoSyncAppDelegate.mm"
FileUtils.cp "Classes/MoSyncViewController.h", "template/Classes/MoSyncViewController.h"
FileUtils.cp "Classes/MoSyncViewController.m", "template/Classes/MoSyncViewController.m"

# copy template files and default icons
FileUtils.cp "library_project/project.plisttemplate", "template/project.plisttemplate"
FileUtils.cp "library_project/project.pbxprojtemplate", "template/project.pbxprojtemplate"
FileUtils.cp "icon_72x72.png", "template/icon_72x72.png"
FileUtils.cp "icon_57x57.png", "template/icon_57x57.png"
FileUtils.cp "MoSync_Prefix.pch", "template/MoSync_Prefix.pch"
FileUtils.cp "main.m", "template/main.m"
FileUtils.cp "../../../../intlibs/helpers/cpp_maapi.h", "template/Classes/helpers/cpp_maapi.h"
FileUtils.cp "../../../../intlibs/helpers/cpp_defs.h", "template/Classes/helpers/cpp_defs.h"
FileUtils.cp "../../../../tools/idl2/Output/syscall_static_cpp.h", "template/Classes/syscall_static_cpp.h"
