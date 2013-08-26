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
