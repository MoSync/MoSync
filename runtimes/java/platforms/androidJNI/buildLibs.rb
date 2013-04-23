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
	old_sh(cmd)
	return true
end

def clearDir(path)
if File.exist? path
	rm_rf path
end
mkdir_p path
end

def buildLib(name, buildParams)
	srcfiles = buildParams['src']
	modules = buildParams['modules']
	bootmodules = buildParams['bootmodules'] || "mosync"
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
	if compilerflags != nil && !compilerflags.empty?
		compileflags = "--compiler-switches \"" + compilerflags + " -DANDROID\""
	end
	success = sh("#{ENV['MOSYNCDIR']}/bin/nbuild --platform Android --name #{name} --project #{project} --dst #{ENV['MOSYNCDIR']}/lib --config Debug,Release --lib-variant debug,release --android-ndkbuild-cmd #{$androidNDKPath}/ndk-build --android-version #{$androidVersion} #{srcfiles} --verbose #{libtype} #{includes} #{bootmoduleList} #{moduleList} #{compilerflags} --android-lib-type #{libtype} --android-build-dir #{project}/temp_#{name}")
	if !success
		exit 1
	end
end

$androidNDKPath = ARGV[0]
$androidVersion = ARGV[1]

# Converts the android version to an Integer
androidVersionInt = Integer($androidVersion)

if ENV['MOSYNC_SRC'] == nil
	cd "../../../../"
	ENV['MOSYNC_SRC'] = pwd
end

scriptPath = "#{ENV['MOSYNC_SRC']}/runtimes/java/platforms/androidJNI"
cd scriptPath

#We need two different make files for android due to some restrictions in JNI

puts "android version is: #{$androidVersion}"
if(androidVersionInt < 7)
	ENV['ANDROID_API_BELOW_7'] = "true"
end

if(androidVersionInt >= 15)
	ENV['ANDROID_API_15_OR_ABOVE'] = "true"
end

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
#buildLib("wchar", { 'src' => "-SMAStd/wchar.c -SMAStd/mawstring.c -SMAStd/mawvsprinf.c", 'bootmodules' => "." })

bootfiles = "-S./* -S./UTF/* -S../../core/Core.cpp -S../sdl/FileImpl.cpp -S../../base/FileStream.cpp -S../../base/MemStream.cpp -S../../base/Stream.cpp -S../../base/Image.cpp -S../../base/ResourceArray.cpp -S../../base/Syscall.cpp -S../../../../intlibs/helpers/platforms/linux/log.cpp -S../../../../intlibs/hashmap/hashmap.cpp"
cppDir = "#{ENV['MOSYNC_SRC']}/runtimes/cpp"
additionalIncludes = "-I#{cppDir} -I#{cppDir}/base -I#{cppDir}/core -I#{ENV['MOSYNC_SRC']}/intlibs"
buildLib("mosync", { 'src' => bootfiles, 'compiler-flags' => "-Wno-psabi -D_android", 'project' => "#{cppDir}/platforms/android", 'includes' => additionalIncludes, 'bootmodules' => "."})
#sh("#{ENV['MOSYNCDIR']}/bin/nbuild --platform Android --name mosync --project #{cppDir}/platforms/android --dst #{ENV['MOSYNCDIR']}/lib --config Debug,Release --lib-variant debug,release --android-ndkbuild-cmd #{$androidNDKPath}/ndk-build --android-version #{$androidVersion} #{bootfiles} --verbose --boot-modules . #{additionalIncludes} --compiler-switches -Wno-psabi -D_android")

#wcharfiles = "-S./newlib/libc/string/wcpcpy.c -S./newlib/libc/string/wcsncpy.c -S./newlib/libc/string/wcpncpy.c -S./newlib/libc/string/wcsnlen.c -S./newlib/libc/string/wcscasecmp.c -S./newlib/libc/string/wcspbrk.c -S./newlib/libc/string/wcscat.c -S./newlib/libc/string/wcsrchr.c -S./newlib/libc/string/wcschr.c -S./newlib/libc/string/wcsspn.c -S./newlib/libc/string/wcscmp.c -S./newlib/libc/string/wcsstr.c -S./newlib/libc/string/wcscoll.c -S./newlib/libc/string/wcstok.c -S./newlib/libc/string/wcscpy.c -S./newlib/libc/string/wcswidth.c -S./newlib/libc/string/wcscspn.c -S./newlib/libc/string/wcsxfrm.c -S./newlib/libc/string/wcsdup.c -S./newlib/libc/string/wcwidth.c -S./newlib/libc/string/wcslcat.c -S./newlib/libc/string/wmemchr.c -S./newlib/libc/string/wcslcpy.c -S./newlib/libc/string/wmemcmp.c -S./newlib/libc/string/wcslen.c -S./newlib/libc/string/wmemcpy.c -S./newlib/libc/string/wcsncasecmp.c -S./newlib/libc/string/wmemmove.c -S./newlib/libc/string/wcsncat.c -S./newlib/libc/string/wmemset.c -S./newlib/libc/string/wcsncmp.c"
#buildLib("MAStd", { 'src' => "-Skazlib -SMinUI -SMAMath/MAVector3.c -SResCompiler -SMAStd/conprint.c -SMAStd/maassert.c -SMAStd/mastring.c -SMAStd/matime.c -SMAStd/mavsprintf.c -SMAStd/mawvsprintf.c -SMAStd/maxtoa.c -SMAStd/wchar.c -SMAStd/mawstring.c -SMAStd/mawvsprinf.c" })
#buildLib("mautil", { 'src' => "-SHybris -SMAUtil -XMAUtil/DomParser.cpp -XMAUtil/GraphicsOpenGL.c -XMAUtil/XMLDataProvider.cpp -XMAUtil/XPathTokenizer.cpp", 'modules' => "MAStd" })
#buildLib("maui", { 'src' => "-SMAUI", 'modules' => "MAStd,MAUtil"})
#buildLib("mafs", { 'src' => "-SMAFS/**" })
#buildLib("ads", { 'src' => "-SAds", 'modules' => "MAStd,MAUtil" })
#buildLib("Facebook", { 'src' => "-SFacebook/**", 'modules' => "MAStd,MAUtil" })
#buildLib("nativeui", { 'src' => "-SNativeUI", 'modules' => "MAStd,MAUtil" })
#buildLib("Purchase", { 'src' => "-SPurchase", 'modules' => "MAStd,MAUtil" })
#buildLib("Wormhole", { 'src' => "-SWormhole/**", 'modules' => "MAStd,MAUtil" })
#buildLib("matest", { 'src' => "-SMATest" })
#buildLib("mtxml", { 'src' => "-SMTXml", 'modules' => "MAStd,MAUtil" })
#buildLib("testify", { 'src' => "-STestify/src" })
#buildLib("yajl", { 'src' => "-Syajl/**", 'modules' => "MAStd,MAUtil" })
#buildLib("Notification", { 'src' => "-SNotification/**", 'modules' => "MAStd,MAUtil" })

libfiles = "-SMAUI -SMAStd/conprint.c -SMAStd/maassert.c -SMAStd/mastring.c -SMAStd/mawstring.c -SMAStd/matime.c -SMAStd/mavsprintf.c -SMAStd/mawvsprintf.c -SMAStd/maxtoa.c -SMAStd/wchar.c -SMAMath/MAVector3.c -SAds -SMinUI -SResCompiler -Skazlib -Syasper -SFacebook/** -SNativeUI -SHybris -SMAFS/** -SMAUtil -XMAUtil/DomParser.cpp -SNotification -STestify -SMATest -SPurchase -SWormhole/** -Syajl/** -XMAUtil/GraphicsOpenGL.c -XMAUtil/XMLDataProvider.cpp -XMAUtil/XPathTokenizer.cpp"
#libfiles = "- -Skazlib  -SHybris "
buildLib("mosynclib", { 'shared' => true, 'src' => libfiles, 'compiler-flags' => "-D_MB_CAPABLE" })
