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

require "#{File.dirname(__FILE__)}/native_link.rb"

# Links object files together to form a native executable.
class ExeTask < NativeGccLinkTask
	def initialize(work, name, objects, whole_libs, libs, linkflags)
		super(work, name, whole_libs + objects + libs)
		@FLAGS = linkflags
	end
end

# Compiles C/C++ code into an native executable file.
# Supports GCC on linux and mingw.
class ExeWork < NativeGccLinkWork
	def link_task_class
		ExeTask
	end
	def link_file_ending
		EXE_FILE_ENDING
	end
end
