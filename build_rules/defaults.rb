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

require "#{BD}/build_rules/util.rb"

default(:CONFIG, "debug")

default(:EXTRA_CFLAGS, "")
default(:EXTRA_CXXFLAGS, "")
default(:EXTRA_EXEFLAGS, "")
default(:EXTRA_DLLFLAGS, "")
default(:EXTRA_INCLUDES, [])
default(:IGNORED_FILES, [])
default(:EXTRA_SOURCEFILES, [])
default(:EXTRA_OBJECTS, [])
default(:LIBRARIES, [])
default(:LOCAL_LIBS, [])
default(:LOCAL_DLLS, [])
default(:SPECIFIC_CFLAGS, {})
default(:PIPEFLAGS, "")
default(:DEFAULT_LIBS, ["mastd"])
default(:CUSTOM_LIBS, [])
default(:BUILDDIR_BASE, "build/")
default(:EXTRA_HEADERFILES, [])
default(:IGNORED_HEADERS, [])
default(:PIPE_ONLY, false)

if(CONFIG == "debug") then
	BUILDDIR = BUILDDIR_BASE + "debug/"
	CONFIG_POSTFIX = "D"
elsif(CONFIG == "")
	BUILDDIR = BUILDDIR_BASE
	CONFIG_POSTFIX = ""
else
	error "wrong configuration: " + CONFIG
end

BUILDPATH = "#{BD}/#{BUILDDIR}"
