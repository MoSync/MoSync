
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

pedantic_warnings = " -Wunreachable-code -Wmissing-noreturn -Wmissing-format-attribute"

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
	end
else
	base_flags = ""
end
if(!GCC_IS_V43) then
	flag_warnings = ""
	lesser_conly += gcc43_c_warnings
end

if(@CONFIG == "debug") then
	config_flags = " -g -O0"
elsif(@CONFIG == "")
	config_flags =  " -O2"
else
	error "wrong configuration: " + @CONFIG
end

flags_base = config_flags + base_flags + include_flags + standard_warnings + lesser_warnings +
	pedantic_warnings + flag_warnings

cflags_base = flags_base + lesser_conly + pendantic_c_warnings

cppflags_base = " -fno-rtti" + flags_base
# -Wno-deprecated

@CFLAGS = cflags_base + @EXTRA_CFLAGS
@CPPFLAGS = cppflags_base + @EXTRA_CPPFLAGS
end
end
