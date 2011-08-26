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

# Links object files together to form a native static library.
# Inherits from LinkTask to share flag-handling code.
class NativeLibTask < NativeGccLinkTask
	def initialize(work, name, objects)
		super(work, name, objects, nil)
	end
	
	def execute
		execFlags
		# ar does not remove out-of-date archive members.
		# The file must be deleted if we are to get a clean build.
		FileUtils.rm_f(@NAME)
		sh "ar rcs #{@NAME} #{cFlags}"
	end
end

# Compiles C/C++ code into a native static library.
# Supports GCC on linux and mingw.
# In addition to the variables used by GccWork, this class uses the following:
# @NAME and @COMMON_BUILDDIR.
class NativeLibWork < NativeGccWork
	private
	
	def setup3(all_objects, have_cppfiles)
		#puts @COMMON_BUILDDIR
		target = @COMMON_BUILDDIR + @NAME + NATIVE_LIB_FILE_ENDING
		dirTask = DirTask.new(self, @COMMON_BUILDDIR)
		@TARGET = NativeLibTask.new(self, target, all_objects)
		@prerequisites += [dirTask, @TARGET]
 	end
end
