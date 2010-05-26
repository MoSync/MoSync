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
include_flags = include_dirs.collect {|dir| " -I "+dir}.join

#temp
#flag_warnings = gcc4_warnings + gcc43_c_warnings + gcc43_warnings

c_flags = " -std=gnu99"

version_warnings = ""
base_flags = ""
cpp_flags = ""
if(@GCC_IS_V4) then
	if(HOST != :win32)
		base_flags += " -fvisibility=hidden"
	end
	version_warnings += gcc4_warnings
	if(@GCC_IS_V43 || @GCC_IS_V44) then
		version_warnings += gcc43_c_warnings + gcc43_warnings
		cpp_flags += " -std=gnu++0x -DHAVE_TR1"
	end
end
if(!@GCC_IS_V43) then
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
	@HOST_FLAGS = " -DLINUX"
	if(HOST_PLATFORM == :darwin)
		@HOST_FLAGS += " -m32 -DDARWIN"
	end
	@HOST_CPPFLAGS = " -fPIC"
elsif(HOST == :darwin)
	@HOST_FLAGS = " -m32 -DDARWIN"
	@HOST_CPPFLAGS = " -m32 -fPIC"
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
