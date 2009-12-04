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
