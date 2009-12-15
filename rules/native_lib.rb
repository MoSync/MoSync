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

require "#{File.dirname(__FILE__)}/native_gcc.rb"

# Links object files together to form a native static library.
class NativeLibTask < FileTask
	def initialize(work, name, objects)
		super(work, name)
		@prerequisites = @objects = objects
	end
	
	def execute
		sh "ar rcs #{@NAME} #{@objects.join(' ')}"
	end
end

# Compiles C/C++ code into a native static library.
# Supports GCC on linux and mingw.
# In addition to the variables used by GccWork, this class uses the following:
# @NAME and @COMMON_BUILDDIR.
class NativeLibWork < NativeGccWork
	private
	
	def setup3(all_objects)
		#puts @COMMON_BUILDDIR
		target = @COMMON_BUILDDIR + @NAME + NATIVE_LIB_FILE_ENDING
		dirTask = DirTask.new(self, @COMMON_BUILDDIR)
		@TARGET = NativeLibTask.new(self, target, all_objects)
		@prerequisites += [dirTask, @TARGET]
 	end
end
