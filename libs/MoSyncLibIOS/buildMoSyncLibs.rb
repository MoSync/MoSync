#!/usr/bin/ruby

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