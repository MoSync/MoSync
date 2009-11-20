
#defines @CFLAGS and @CPPFLAGS

module GccFlags
def define_cflags

#only valid in GCC 4.3 and later
gcc43_warnings = " -Wvla -Wlogical-op"

#valid in both C and C++ in GCC 4.3, but only in C in 4.2 and earlier
gcc43_c_warnings = " -Wmissing-declarations"

#only valid in GCC 4.0 and later
gcc4_warnings = " -Wvariadic-macros -Wmissing-include-dirs"

lesser_warnings = " -Winline -Wpointer-arith -Wundef -Wfloat-equal -Winit-self"

pedantic_warnings = " -Wmissing-noreturn -Wmissing-format-attribute"

#broken in GCC 4.3.3
gcc_not_4_warnings = " -Wunreachable-code"

#only valid in c.
pendantic_c_warnings = " -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes"

#only valid in C.
lesser_conly = " -Wnested-externs -Wdeclaration-after-statement"
# -Wno-format-zero-length"

standard_warnings = " -Wall -Werror -Wextra -Wno-unused-parameter -Wwrite-strings -Wshadow"


include_dirs = ["."] + @EXTRA_INCLUDES
include_flags = include_dirs.collect {|dir| " -I "+dir}.join

#temp
#flag_warnings = gcc4_warnings + gcc43_c_warnings + gcc43_warnings

if(GCC_IS_V4) then
	base_flags = " -fvisibility=hidden"
	flag_warnings = gcc4_warnings
	if(GCC_IS_V43) then
		flag_warnings += gcc43_c_warnings + gcc43_warnings
		cpp_flags = " -std=c++0x -DHAVE_TR1"
	end
else
	base_flags = ""
	cpp_flags = ""
end
if(!GCC_IS_V43) then
	flag_warnings = gcc_not_4_warnings
	lesser_conly += gcc43_c_warnings
end

if(@CONFIG == "debug") then
	config_flags = " -g -O0"
elsif(@CONFIG == "")
	config_flags =  " -O2"
else
	error "wrong configuration: " + @CONFIG
end

if(HOST == :win32)
	host_flags = " -DWIN32"
	host_cpp_flags = ""
elsif(HOST == :linux)
	host_flags = " -DLINUX"
	host_cpp_flags = " -fPIC"
else
	error "Unsupported host: #{HOST}"
end


flags_base = config_flags + host_flags + base_flags + include_flags + standard_warnings + lesser_warnings +
	pedantic_warnings + flag_warnings

cflags_base = flags_base + lesser_conly + pendantic_c_warnings

cppflags_base = cpp_flags + " -fno-rtti" + host_cpp_flags + flags_base
# -Wno-deprecated

@CFLAGS = cflags_base + @EXTRA_CFLAGS
@CPPFLAGS = cppflags_base + @EXTRA_CPPFLAGS
end
end
