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

require "#{File.dirname(__FILE__)}/native_link.rb"

# Links object files together to form a native dynamic-link library.
class DllTask < NativeGccLinkTask
	def initialize(work, name, objects, whole_libs, libs, linkflags, linker)
		super(work, name, objects, linker)
		@prerequisites += whole_libs + libs
		libfile = File.dirname(@NAME) + "/lib" + File.basename(@NAME, DLL_FILE_ENDING) + ".a"
		libflags = ""
		#puts "libflags: #{libflags}"
		if(whole_libs.size > 0)
			if(HOST == :darwin)
				libflags += " #{whole_libs.join(' ')} -m32"
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
			#libflags += " -s"
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
