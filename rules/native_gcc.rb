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

require "#{File.dirname(__FILE__)}/gcc.rb"

gcc_version = get_gcc_version_string("gcc")
NATIVE_GCC_IS_V4 = (gcc_version[0] == "4"[0])
NATIVE_GCC_IS_V43 = (NATIVE_GCC_IS_V4 && (gcc_version[2] == "3"[0]))
NATIVE_GCC_IS_V44 = (NATIVE_GCC_IS_V4 && (gcc_version[2] == "4"[0]))

# Base class.
class NativeGccWork < GccWork
	def gcc; "gcc"; end
	def gccmode; "-c"; end
	def object_ending; ".o"; end
end
