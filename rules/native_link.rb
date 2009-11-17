require "#{File.dirname(__FILE__)}/native_gcc.rb"

class NativeGccLinkTask < FileTask
	def initialize(work, name, objects)
		super(work, name)
		@prerequisites = @objects = objects
		
		# todo: save linkflags, like CompileGccTask's CFLAGS.
	end
	
	def execute
		sh "g++ #{@objects.join(' ')}#{@LINKFLAGS} -o #{@NAME}"
	end
end

class NativeGccLinkWork < NativeGccWork
	private

	def setup3(all_objects)
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		lld = @LOCAL_DLLS.collect { |ld| FileTask.new(self, @COMMON_BUILDDIR + ld + DLL_FILE_ENDING) }
		@LIBRARIES.each { |l| @EXTRA_LINKFLAGS += " -l" + l }
		need(:@NAME)
		need(:@BUILDDIR)
		need(:@TARGETDIR)
		target = @TARGETDIR + "/" + @BUILDDIR + @NAME + link_file_ending
		@TARGET = link_task_class.new(self, target, all_objects + llo + lld, @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
 	end
end
