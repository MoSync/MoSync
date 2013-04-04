# Copyright (C) 2013 MoSync AB
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

# usage: buildJNI.rb <ANDROID_NDK_PATH> <ANDROID_VERSION>

# <ANDROID_NDK_PATH> 		The path to where the ndk is located i.e. C:/Android/android-ndk-r4
# <ANDROID_VERSION>			The android platform version, e.g. 17

# Environment variables:
# MOSYNCDIR				 	The location of the installed MoSync tool
# MOSYNC_SRC				The location of the MoSync repository; if not set, make sure to cd into the
#							directory of this file

require 'fileutils'
require '../../../../rules/util.rb'

include FileUtils::Verbose

alias :old_sh :sh
def sh(cmd)
	old_sh(cmd)
	return true
end

def clearDir(path)
if File.exist? path
	rm_rf path
end
mkdir_p path
end

androidNDKPath = ARGV[0]
androidVersion = ARGV[1]

# Converts the android version to an Integer
androidVersionInt = Integer(androidVersion)

if ENV['MOSYNC_SRC'] == nil
	cd "../../../../"
	ENV['MOSYNC_SRC'] = pwd
end

scriptPath = "#{ENV['MOSYNC_SRC']}/runtimes/java/platforms/androidJNI"
cd scriptPath

#We need two different make files for android due to some restrictions in JNI

puts "android version is: #{androidVersion}"
if(androidVersionInt < 7)
	ENV['ANDROID_API_BELOW_7'] = "true"
end

if(androidVersionInt >= 15)
	ENV['ANDROID_API_15_OR_ABOVE'] = "true"
end

if androidNDKPath == nil
	puts "missing argument, android NDK path is unknown!"
	exit 1
end

if androidVersion == nil
	puts "missing argument, android version is unknown!"
	exit 1
end

if ENV['MOSYNCDIR'] == nil
	puts "MOSYNCDIR environment variable not set!"
	exit 1
end

puts "Building native Library\n\n"

# Now build the native one and put it where it should be
# However, we should merge this using Fredrik's approach soon.
bootfiles = "-S./* -S../../core/Core.cpp -S../sdl/FileImpl.cpp -S../../base/FileStream.cpp -S../../base/MemStream.cpp -S../../base/Stream.cpp -S../../base/Image.cpp -S../../base/ResourceArray.cpp -S../../base/Syscall.cpp -S../../../../intlibs/helpers/platforms/linux/log.cpp -S../../../../intlibs/hashmap/hashmap.cpp"
cppDir = "#{ENV['MOSYNC_SRC']}/runtimes/cpp"
additionalIncludes = "#{cppDir} #{cppDir}/base #{cppDir}/core #{ENV['MOSYNC_SRC']}/intlibs"
sh("#{ENV['MOSYNCDIR']}/bin/nbuild --platform Android --name mosync --project #{cppDir}/platforms/android --dst #{ENV['MOSYNCDIR']}/lib --config Debug,Release --lib-variant debug,release --android-ndkbuild-cmd #{androidNDKPath}/ndk-build --android-version #{androidVersion} #{bootfiles} --verbose --boot-modules . --additional-includes \"#{additionalIncludes}\" --compiler-switches -Wno-psabi -D_android")

libfiles = "-SMAStd/maassert.c -SMAStd/mastring.c -SMAStd/mawstring.c -SMAStd/matime.c -SMAStd/mavsprintf.c -SMAStd/mawvsprintf.c -SMAStd/maxtoa.c -SMAMath/MAVector3.c -SAds -SMinUI -SResCompiler -Skazlib -Syasper -SFacebook/** -SNativeUI -SHybris -SMAFS/** -SMAUtil -XMAUtil/DomParser.cpp -SNotification -STestify -SMATest -SPurchase -SWormhole/** -Syajl/** -XMAUtil/GraphicsOpenGL.c -XMAUtil/XMLDataProvider.cpp -XMAUtil/XPathTokenizer.cpp"
sh("#{ENV['MOSYNCDIR']}/bin/nbuild --platform Android --name mosynclib --project #{ENV['MOSYNC_SRC']}/libs --dst #{ENV['MOSYNCDIR']}/lib --config Debug,Release --lib-variant debug,release --android-ndkbuild-cmd #{androidNDKPath}/ndk-build --android-version #{androidVersion} #{libfiles} --verbose --boot-modules mosync")
