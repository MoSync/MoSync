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

# Links object files together to form a native dynamic-link library.
class DllTask < NativeGccLinkTask
	def initialize(work, name, objects, whole_libs, libs, linkflags)
		super(work, name, objects)
		@prerequisites += whole_libs + libs
		libfile = File.dirname(@NAME) + "/lib" + File.basename(@NAME, DLL_FILE_ENDING) + ".a"
		libflags = ""
		#puts "libflags: #{libflags}"
		if(whole_libs.size > 0)
			if(HOST == :linux && HOST_PLATFORM == :darwin)
				libflags += " #{whole_libs.join(' ')}"
			else
				libflags += " -Wl,--whole-archive #{whole_libs.join(' ')} -Wl,--no-whole-archive"
			end
			#puts "libflags: #{libflags}"
		end
		if(libs.size > 0)
			libflags += ' ' + libs.join(' ')
			#puts "libflags: #{libflags}"
		end
		if(CONFIG == "")
			libflags += " -s"
			#puts "libflags: #{libflags}"
		end
		if(HOST == :win32)
			implib = " -Wl,--out-implib=#{libfile}"
		end
		@FLAGS = " -shared#{implib}" + libflags + linkflags
		#puts "libflags: #{libflags}"
		#puts "linkflags: #{linkflags}"
		#puts "@FLAGS: #{@FLAGS}"
	end
end

# Compiles C/C++ code into an dynamic-link library.
# Supports GCC on linux and mingw.
class DllWork < NativeGccLinkWork
	def link_task_class
		DllTask
	end
	def link_file_ending
		DLL_FILE_ENDING
	end
end
