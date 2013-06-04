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

# usage: buildLibs.rb <ANDROID_NDK_PATH> <ANDROID_VERSION>

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
	old_sh(cmd) do |ok, res|
		if (!ok)
			exit 1
		end
	end
	return true
end

def clearDir(path)
if File.exist? path
	rm_rf path
end
mkdir_p path
end

def buildLib(name, buildParams)
	if (name == $libsPattern || $libsPattern == "." || $libsPattern == nil)
		srcfiles = buildParams['src']
		modules = buildParams['modules']
		bootmodules = buildParams['bootmodules'] || ""
		project = buildParams['project'] || "#{ENV['MOSYNC_SRC']}/libs"
		includes = buildParams['includes'] || ""
		libtype = buildParams['libtype'] || "shared"
		compilerflags = buildParams['compiler-flags'] || ""

		# Todo: move into workfiles
		moduleList = ""
		if modules != nil && !modules.empty?
			moduleList = "--modules " + modules
		end
		bootmoduleList = ""
		if bootmodules != nil && !bootmodules.empty?
			bootmoduleList = "--boot-modules " + bootmodules
		end

		compileflags = "--compiler-switches \"--std=c99 #{compilerflags}\""

		mkdir_p "#{ENV['MOSYNCDIR']}/lib"
		success = sh("#{ENV['MOSYNCDIR']}/bin/nbuild --clean --platform Android --name #{name} --project #{project} --dst #{ENV['MOSYNCDIR']}/lib --config Debug,Release --lib-variant debug,release --android-ndk-location #{$androidNDKPath} --android-version #{$androidVersion} #{srcfiles} --verbose #{libtype} #{includes} #{bootmoduleList} #{moduleList} #{compileflags} --android-lib-type #{libtype} --android-build-dir #{project}/temp_#{name}")
		if !success
			exit 1
		end
	end
end

$androidNDKPath = ARGV[0]
$androidVersion = ARGV[1]
$libsPattern = ARGV[2]
$stl = ARGV[3] == "stlport"

if ($androidNDKPath == '@')
	# Then use the 'settings' ruby file instead.
	settingsFile = "#{File.dirname(__FILE__)}/NdkSettings.rb"
	if !File.exist? settingsFile
		FileUtils.copy_file(settingsFile + ".example", settingsFile)
	end
	require "NdkSettings"

	$androidNDKPath = $SETTINGS[:android_ndk]
	$androidVersion = $SETTINGS[:android_version]
end

if ENV['MOSYNC_SRC'] == nil
	cd "../../../../"
	ENV['MOSYNC_SRC'] = pwd
end

scriptPath = "#{ENV['MOSYNC_SRC']}/runtimes/java/platforms/androidJNI"
cd scriptPath

if $androidNDKPath == nil
	puts "missing argument, android NDK path is unknown!"
	exit 1
end

if $androidVersion == nil
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

# First, we need wchar support
#buildLib("wchar", { 'src' => "-SMAStd/wchar.c -SMAStd/mawstring.c -SMAStd/mawvsprintf.c", 'bootmodules' => "." })

modules = ""
if ($stl)
	modules = 'stlport'
end

bootfiles = "-S./* -S./UTF/* -S../../core/Core.cpp -S../sdl/FileImpl.cpp -S../../base/FileStream.cpp -S../../base/MemStream.cpp -S../../base/Stream.cpp -S../../base/Image.cpp -S../../base/ResourceArray.cpp -S../../base/Syscall.cpp -S../../../../intlibs/helpers/platforms/linux/log.cpp -S../../../../intlibs/hashmap/hashmap.cpp"
cppDir = "#{ENV['MOSYNC_SRC']}/runtimes/cpp"
additionalIncludes = "-I#{cppDir} -I#{cppDir}/base -I#{cppDir}/core -I#{ENV['MOSYNC_SRC']}/intlibs"
buildLib("mosync", {
	'modules' =>  modules,
	'src' => bootfiles,
	'compiler-flags' =>	"-Wno-psabi -D_android",
	'project' => "#{cppDir}/platforms/android",
	'includes' => additionalIncludes,
	'bootmodules' => "." })
#sh("#{ENV['MOSYNCDIR']}/bin/nbuild --platform Android --name mosync --project #{cppDir}/platforms/android --dst #{ENV['MOSYNCDIR']}/lib --config Debug,Release --lib-variant debug,release --android-ndk-location #{$androidNDKPath} --android-version #{$androidVersion} #{bootfiles} --verbose --boot-modules . #{additionalIncludes} --compiler-switches -Wno-psabi -D_android")

#wcharfiles = "-S./newlib/libc/string/wcpcpy.c -S./newlib/libc/string/wcsncpy.c -S./newlib/libc/string/wcpncpy.c -S./newlib/libc/string/wcsnlen.c -S./newlib/libc/string/wcscasecmp.c -S./newlib/libc/string/wcspbrk.c -S./newlib/libc/string/wcscat.c -S./newlib/libc/string/wcsrchr.c -S./newlib/libc/string/wcschr.c -S./newlib/libc/string/wcsspn.c -S./newlib/libc/string/wcscmp.c -S./newlib/libc/string/wcsstr.c -S./newlib/libc/string/wcscoll.c -S./newlib/libc/string/wcstok.c -S./newlib/libc/string/wcscpy.c -S./newlib/libc/string/wcswidth.c -S./newlib/libc/string/wcscspn.c -S./newlib/libc/string/wcsxfrm.c -S./newlib/libc/string/wcsdup.c -S./newlib/libc/string/wcwidth.c -S./newlib/libc/string/wcslcat.c -S./newlib/libc/string/wmemchr.c -S./newlib/libc/string/wcslcpy.c -S./newlib/libc/string/wmemcmp.c -S./newlib/libc/string/wcslen.c -S./newlib/libc/string/wmemcpy.c -S./newlib/libc/string/wcsncasecmp.c -S./newlib/libc/string/wmemmove.c -S./newlib/libc/string/wcsncat.c -S./newlib/libc/string/wmemset.c -S./newlib/libc/string/wcsncmp.c"
#buildLib("MAStd", { 'src' => "-Skazlib -SMinUI -SMAMath/MAVector3.c -SResCompiler -SMAStd/conprint.c -SMAStd/maassert.c -SMAStd/mastring.c -SMAStd/matime.c -SMAStd/mavsprintf.c -SMAStd/mawvsprintf.c -SMAStd/maxtoa.c -SMAStd/wchar.c -SMAStd/mawstring.c -SMAStd/mawvsprinf.c" })

#libfiles = "-SMAP -SMAUI -SMAStd/conprint.c -SMAStd/maassert.c -SMAStd/mastring.c -SMAStd/mawstring.c -SMAStd/matime.c -SMAStd/mavsprintf.c -SMAStd/mawvsprintf.c -SMAStd/maxtoa.c -SMAStd/wchar.c -SMAMath/MAVector3.c -SAds -SMinUI -SResCompiler -Skazlib -Syasper -SFacebook/** -SNativeUI -SHybris -SMAFS/** -SMAUtil -XMAUtil/DomParser.cpp -SNotification -STestify -SMATest -SPurchase -SWormhole/** -Syajl/** -XMAUtil/GraphicsOpenGL.c -XMAUtil/XMLDataProvider.cpp -XMAUtil/XPathTokenizer.cpp"
libfiles = "-SMAStd/conprint.c -SMAStd/maassert.c -SMAStd/mastring.c -SMAStd/mawstring.c -SMAStd/matime.c -SMAStd/mavsprintf.c -SMAStd/mawvsprintf.c -SMAStd/maxtoa.c -SMAStd/wchar.c -SMAMath/MAVector3.c -SResCompiler -Syasper"
#if $stl
#	libfiles = "-SMoGraph/** " + libfiles
#end
#libfiles = "- -Skazlib  -SHybris "
buildLib("mosynclib", {
	'modules' => modules,
	'src' => libfiles,
	'compiler-flags' => "-D_MB_CAPABLE",
	'includes' => "-I#{ENV['MOSYNC_SRC']}/libs",
	'bootmodules' => 'mosync' })

buildLib("mautil", { 'src' => "-Skazlib/dict.c -Skazlib/hash.c -SMAUtil -XMAUtil/DomParser.cpp -XMAUtil/GraphicsOpenGL.c -XMAUtil/XMLDataProvider.cpp -XMAUtil/XPathTokenizer.cpp" })
buildLib("yajl", { 'src' => "-Syajl/**", 'modules' => "mautil" })
buildLib("maui", { 'src' => "-SMAUI -SMinUI", 'modules' => "mautil" })
buildLib("mafs", { 'src' => "-SMAFS/**"})
buildLib("map", { 'src' => "-SMAP/**", 'modules' => "mautil,maui" })
buildLib("ads", { 'src' => "-SAds", 'modules' => "mautil" })
buildLib("nativeui", { 'src' => "-SNativeUI", 'modules' => "mautil" })
buildLib("Facebook", { 'src' => "-SFacebook/**", 'modules' => "mautil,yajl" })
buildLib("Purchase", { 'src' => "-SPurchase", 'modules' => "mautil" })
buildLib("matest", { 'src' => "-SMATest", 'modules' => "mautil" })
# We don't include this one: buildLib("mtxml", { 'src' => "-SMTXml", 'modules' => "mautil" })
buildLib("testify", { 'src' => "-STestify/src", 'includes' => "-I#{ENV['MOSYNC_SRC']}/libs/Testify/inc" })
buildLib("Notification", { 'src' => "-SNotification/**", 'modules' => "mautil" })
buildLib("Wormhole", { 'src' => "-SWormhole/**", 'modules' => "mautil,mafs,yajl,nativeui,Notification" })
buildLib("MoGraph", { 'src' => "-SMoGraph/**", 'modules' => "mautil" })

# We do not ship stlport stuff
File.delete "#{ENV['MOSYNCDIR']}/lib/android_armeabi_debug/libstlport_shared.so"
File.delete "#{ENV['MOSYNCDIR']}/lib/android_armeabi_release/libstlport_shared.so"
File.delete "#{ENV['MOSYNCDIR']}/lib/android_armeabi-v7a_debug/libstlport_shared.so"
File.delete "#{ENV['MOSYNCDIR']}/lib/android_armeabi-v7a_release/libstlport_shared.so"