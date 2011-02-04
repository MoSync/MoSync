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

require "../../../../rules/util.rb"
require "FileUtils"

#FileUtils.rm_rf(["build"])


createTest = false

ARGV.each do |x|
	if (x == "-createTest")
		createTest = true
	end
end


# build everything
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphoneos -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphoneos -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphonesimulator -project MoSync.xcodeproj"
sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphonesimulator -project MoSync.xcodeproj"
#sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphoneos3.2 -project MoSync.xcodeproj"
#sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphoneos3.2 -project MoSync.xcodeproj"
#sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Debug  -sdk iphonesimulator3.2 -project MoSync.xcodeproj"
#sh "/Developer/usr/bin/xcodebuild -target MoSyncLib -configuration Release -sdk iphonesimulator3.2 -project MoSync.xcodeproj"

# build directory structure
FileUtils.rm_rf(["template"])
FileUtils.mkpath(["template/libs", "template/Classes", "template/Classes/helpers"])

# concatenate libraries (removed this don't know how good it works)
#sh "lipo build/Debug-iphoneos/libMoSyncLib.a build/Debug-iphonesimulator/libMoSyncLib.a -create -output template/libs/debug/libMoSyncLib.a"
#sh "lipo build/Release-iphoneos/libMoSyncLib.a build/Release-iphonesimulator/libMoSyncLib.a -create -output template/libs/release/libMoSyncLib.a"
FileUtils.cp_r Dir.glob('build/Debug-*'), 'template/libs', :verbose => true
FileUtils.cp_r Dir.glob('build/Release-*'), 'template/libs', :verbose => true

# these files should probably be combined into one
FileUtils.cp "Classes/converters.h", "template/Classes/converters.h"
FileUtils.cp "Classes/mstypeinfo.h", "template/Classes/mstypeinfo.h"

# these are needed
FileUtils.cp "Classes/MoSyncAppDelegate.h", "template/Classes/MoSyncAppDelegate.h"
FileUtils.cp "Classes/MoSyncAppDelegate.mm", "template/Classes/MoSyncAppDelegate.mm"
FileUtils.cp "Classes/MoSyncViewController.h", "template/Classes/MoSyncViewController.h"
FileUtils.cp "Classes/MoSyncViewController.mm", "template/Classes/MoSyncViewController.mm"

# copy template files and default icons
FileUtils.cp "Icon-72.png", "template"
FileUtils.cp "Icon.png", "template"
FileUtils.cp "MoSync_Prefix.pch", "template/MoSync_Prefix.pch"
FileUtils.cp "main.m", "template/main.m"
FileUtils.cp "../../../../intlibs/helpers/maapi_defs.h", "template/Classes/helpers/maapi_defs.h"
FileUtils.cp "../../../../intlibs/helpers/cpp_maapi.h", "template/Classes/helpers/cpp_maapi.h"
FileUtils.cp "../../../../intlibs/helpers/cpp_defs.h", "template/Classes/helpers/cpp_defs.h"
FileUtils.cp "../../../../tools/idl2/Output/syscall_static_cpp.h", "template/Classes/syscall_static_cpp.h"

# copy template files
FileUtils.cp "library_project/__PROJECT_NAME__.plist", "template/project.plisttemplate"
FileUtils.cp "library_project/__PROJECT_NAME__.xcodeproj/project.pbxproj", "template/project.pbxprojtemplate"

# make template test directory
if (createTest == true)
	FileUtils.rm_rf(["template_test"])
	FileUtils.mkpath(["template_test"])
	FileUtils.cp "library_project/__PROJECT_NAME__.plist", "template_test"
	FileUtils.cp_r "library_project/__PROJECT_NAME__.xcodeproj", "template_test"

	FileUtils.cp_r Dir.glob('template/*'), "template_test"
	FileUtils.cp ["resources", "data_section.bin"], "template_test"
	FileUtils.cp "Classes/rebuild.build.cpp", "template_test/Classes"
end
