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

# Links object files together to form a native executable.
class ExeTask < NativeGccLinkTask
	def initialize(work, name, objects, whole_libs, libs, linkflags, linker)
		super(work, name, whole_libs + objects + libs, linker)
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
