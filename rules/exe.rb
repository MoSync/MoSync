require "#{File.dirname(__FILE__)}/native_gcc.rb"

class ExeTask < FileTask
	def initialize(work, name, objects, linkflags)
		super(work, name)
		@prerequisites = @objects = objects
		@LINKFLAGS = linkflags
		
		# todo: save linkflags, like CompileGccTask's CFLAGS.
	end
	
	def execute
		sh "g++ #{@objects.join(' ')}#{@LINKFLAGS} -o #{@NAME}"
	end
end

# Compiles C/C++ code into an executable file.
# Supports GCC on linux and mingw
class ExeWork < NativeGccWork
	private

	def setup3(all_objects)
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		lld = @LOCAL_DLLS.collect { |ld| FileTask.new(self, @COMMON_BUILDDIR + ld + DLL_FILE_ENDING) }
		need(:@NAME)
		need(:@BUILDDIR)
		need(:@TARGETDIR)
		target = @TARGETDIR + "/" + @BUILDDIR + @NAME + EXE_FILE_ENDING
		@TARGET = ExeTask.new(self, target, all_objects + llo + lld, @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
 	end
end
