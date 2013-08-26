# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require "#{File.dirname(__FILE__)}/util.rb"

#defines @CFLAGS and @CPPFLAGS

module GccFlags
def define_cflags

#only valid in GCC 4.3 and later
gcc43_warnings = " -Wvla -Wlogical-op"

#valid in both C and C++ in GCC 4.3, but only in C in 4.2 and earlier
gcc43_c_warnings = " -Wmissing-declarations"

#only valid in GCC 4.0 and later
gcc4_warnings = " -Wvariadic-macros -Wmissing-include-dirs"

lesser_warnings = " -Wpointer-arith -Wundef -Wfloat-equal -Winit-self"

pedantic_warnings = " -Wmissing-noreturn -Wmissing-format-attribute"

#only valid in c.
pendantic_c_warnings = " -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes"

#only valid in C.
lesser_conly = " -Wnested-externs -Wdeclaration-after-statement"
# -Wno-format-zero-length"

#broken in C++, GCC 4.3.3, 4.4.1, 4.2.1 and in 3.4.5 -O2.
optimizer_dependent = " -Wunreachable-code -Winline"
if(@GCC_IS_V4 || (!@GCC_IS_V4 && CONFIG == ""))
	pendantic_c_warnings += optimizer_dependent
else
	pedantic_warnings += optimizer_dependent
end

standard_warnings = " -Wall -Werror -Wextra -Wno-unused-parameter -Wwrite-strings -Wshadow"


include_dirs = @EXTRA_INCLUDES
include_flags = include_dirs.collect {|dir| " -I \""+File.expand_path_fix(dir)+'"'}.join

#temp
#flag_warnings = gcc4_warnings + gcc43_c_warnings + gcc43_warnings

c_flags = " -std=gnu99"

version_warnings = ""
base_flags = ""
cpp_flags = ""

if(PROFILING)
	base_flags += " -pg"
end

if(@GCC_IS_CLANG)
	base_flags << ' -ccc-host-triple mapip-unknown-unknown -ccc-clang-archs mapip -DMAPIP -v'
	version_warnings << ' -Wno-error'
end

if(@GCC_IS_V4) then
	if(HOST != :win32)
		base_flags += " -fvisibility=hidden"
	end
	version_warnings += gcc4_warnings
	if(@GCC_V4_SUB >= 3) then
		version_warnings += gcc43_c_warnings + gcc43_warnings
		cpp_flags += " -std=gnu++0x -DHAVE_TR1"
	end
end
if(!(@GCC_IS_V4 && @GCC_V4_SUB >= 3)) then
	lesser_conly += gcc43_c_warnings
end

if(CONFIG == "debug") then
	config_flags = " -g -O0"
elsif(CONFIG == "")
	config_flags =  " -O2"
else
	error "wrong configuration: " + CONFIG
end

if(HOST == :win32)
	@HOST_FLAGS = " -DWIN32"
	@HOST_CPPFLAGS = ""
elsif(HOST == :linux)
	@HOST_FLAGS = " -DLINUX -fPIC"
	if(HOST_PLATFORM == :darwin)
		sdkNumber = (File.exist?("/Developer/SDKs/MacOSX10.5.sdk")) ? "5":"6"
		sdkAdress = "/Developer/SDKs/MacOSX10.#{sdkNumber}.sdk"
		@HOST_FLAGS += " -isysroot #{sdkAdress} -mmacosx-version-min=10.5 -m32 -DDARWIN"
	end
	@HOST_CPPFLAGS = ""
elsif(HOST == :darwin)
	sdkAdress = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk"
	if(!File.exist?(sdkAdress))
		sdkNumber = (File.exist?("/Developer/SDKs/MacOSX10.5.sdk")) ? "5":"6"
		sdkAdress = "/Developer/SDKs/MacOSX10.#{sdkNumber}.sdk"
	end
	@HOST_FLAGS = " -isysroot #{sdkAdress} -mmacosx-version-min=10.5 -m32 -DDARWIN"
	@HOST_CPPFLAGS = " -isysroot #{sdkAdress} -mmacosx-version-min=10.5 -m32 -fPIC"
else
	error "Unsupported host: #{HOST}"
end


flags_base = config_flags + base_flags + include_flags + standard_warnings + lesser_warnings +
	pedantic_warnings + version_warnings

cflags_base = c_flags + flags_base + lesser_conly + pendantic_c_warnings

cppflags_base = cpp_flags + " -fno-rtti" + flags_base
# -Wno-deprecated

@CFLAGS = cflags_base + @EXTRA_CFLAGS
@CPPFLAGS = cppflags_base + @EXTRA_CPPFLAGS
end
end
