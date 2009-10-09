# Copyright (C) 2009 Mobile Sorcery AB
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

require "#{BD}/build_rules/defaults.rb"
require "#{BD}/build_rules/host.rb"

MACHDEP = " -D" + HOST.upcase +
	" -D_POSIX_SOURCE" #avoid silly bsd functions

if(HOST == "win32") then
	INTRA_INCLUDES = ["#{BD}/tools/ReleasePackageBuild/build_package_tools/include"]
	LIB_OBJECTS = CUSTOM_LIBS.collect do |lib| "#{BD}/tools/ReleasePackageBuild/build_package_tools/lib/#{lib}" end
else
	INTRA_INCLUDES = []
	LIB_OBJECTS = []
end

INCLUDE_DIRS = ["#{BD}/intlibs", "#{BD}/intlibs/helpers", "#{BD}/libs"] +
	((defined?(MOSYNC) == nil) ? [] : ["#{BD}/libs/MAStd"]) + INTRA_INCLUDES + EXTRA_INCLUDES

GCC4_WARNINGS = " -Wvariadic-macros -Wmissing-include-dirs -Wstrict-overflow=0"
GCC43_WARNINGS = " -Wvla -Wlogical-op"

#valid in both C and C++ in GCC 4.3, but only in C in 4.2
GCC43_C_WARNINGS = " -Wmissing-declarations"

if(GCC_IS_V4) then
	if(GCC_IS_V43) then
		GCC_WARNINGS = GCC4_WARNINGS + GCC43_WARNINGS + GCC43_C_WARNINGS
		LESSER_CONLY2 = ""
	else
		GCC_WARNINGS = GCC4_WARNINGS + GCC43_WARNINGS
		LESSER_CONLY2 = GCC43_C_WARNINGS
	end
else
	LESSER_CONLY2 = ""
	GCC_WARNINGS = ""
end

INTRA_CFLAGS = GCC_WARNINGS + " -c" + EXTRA_CFLAGS
OBJECT_ENDING = ".o"
GPP = "g++"
GCC = "gcc"

INTRA_CXXFLAGS = ((defined?(MOSYNC) == nil) ? " -fexceptions" : " -fno-exceptions") + EXTRA_CXXFLAGS

INTRA_OBJECTS = LOCAL_LIBS.collect do |lib| "#{BUILDPATH}lib#{lib}.a" end +
	(LOCAL_DLLS + ((defined?(MOSYNC) == nil) ? [] : ["mosync", "mastd"])).collect do
		|dll| File.expand_path("#{BUILDPATH}lib#{dll}#{DLL_FILE_ENDING}") end +
	LIB_OBJECTS

EXE_OBJECTS = ((defined?(MOSYNC) == nil) ? [] : ["#{BUILDPATH}mosyncmain.o"])

if(HOST == "win32") then
	PRE_LINKFLAGS = " -Wl,--enable-auto-import"
else
	PRE_LINKFLAGS = ""
end

INTRA_EXEFLAGS = (LIBRARIES.collect do |lib| " -l#{lib}" end).to_s

def build_exe(t)
	sh "g++#{PRE_LINKFLAGS} #{t.prerequisites}#{EXEFLAGS} -o #{t.name}"
end

def build_lib(t)
	sh "ar rcs #{t.name} #{t.prerequisites}"
end

def build_dll(t)
	sh "g++#{PRE_LINKFLAGS} #{t.prerequisites}#{DLLFLAGS} -o #{t.name}"
end

if(defined?(LIBNAME) != nil) then
	LIBTARGET = "#{BD}/#{BUILDDIR}lib#{LIBNAME}.a"
end


require "#{BD}/build_rules/common.rb"

#DLLFLAGS = " -shared -Xlinker --no-undefined" + EXTRA_DLLFLAGS + INTRA_EXEFLAGS
DLLFLAGS = " -shared" + EXTRA_DLLFLAGS + INTRA_EXEFLAGS
EXEFLAGS = INTRA_EXEFLAGS


OBJECTS.each do |s|
	OBJECTS.each do |t|
		if(t != s && File.basename(s) == File.basename(t)) then
			raise("Duplicate filename: " + t + " & " + s)
		end
	end
end

if(defined?(DISTRIB_DLLNAME) != nil && defined?(DLLTARGET) == nil) then
	DLLTARGET = "#{MOSYNCDIR}/lib/#{DISTRIB_DLLNAME}#{DLL_FILE_ENDING}"
end

if(defined?(DISTRIB_EXENAME) != nil && defined?(EXETARGET) == nil) then
	EXETARGET = "#{MOSYNCDIR}/bin/#{DISTRIB_EXENAME}#{EXE_FILE_ENDING}"
end

if(defined?(EXENAME) != nil && defined?(EXETARGET) == nil) then
	EXETARGET = "#{BD}/#{BUILDDIR}#{EXENAME}#{EXE_FILE_ENDING}"
end

if(defined?(EXETARGET) != nil) then
	myTask = file EXETARGET => OBJECTS + EXE_OBJECTS do |t|
		build_exe(t)
	end
	task :default => myTask
	task :clean do
		sh "rm -rf #{EXETARGET}"
	end
end

if(defined?(DLLNAME) != nil) then
	DLLTARGET = "#{BD}/#{BUILDDIR}lib#{DLLNAME}#{DLL_FILE_ENDING}"
end

if(defined?(DLLTARGET) != nil) then
	myTask = file DLLTARGET => OBJECTS do |t|
		build_dll(t)
	end
	task :default => myTask
	task :clean do
		sh "rm -rf #{DLLTARGET}"
	end
end
