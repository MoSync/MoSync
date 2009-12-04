require "#{File.dirname(__FILE__)}/native_gcc.rb"
require "#{File.dirname(__FILE__)}/flags.rb"

class NativeGccLinkTask < FileTask
	def initialize(work, name, objects)
		super(work, name)
		initFlags
		@prerequisites = @objects = objects
	end
	
	def needed?(log = true)
		return true if(flagsNeeded?(log))
		super(log)
	end
	
	def execute
		execFlags
		sh "g++#{@FLAGS} #{@objects.join(' ')} -o #{@NAME}"
	end
	
	include FlagsChanged
end

class NativeGccLinkWork < NativeGccWork
	private

	def setup3(all_objects)
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		lld = @LOCAL_DLLS.collect { |ld| FileTask.new(self, @COMMON_BUILDDIR + ld + DLL_FILE_ENDING) }
		wlo = @WHOLE_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".a") }
		@LIBRARIES.each { |l| @EXTRA_LINKFLAGS += " -l" + l }
		need(:@NAME)
		need(:@BUILDDIR)
		need(:@TARGETDIR)
		target = @TARGETDIR + "/" + @BUILDDIR + @NAME + link_file_ending
		@TARGET = link_task_class.new(self, target, all_objects, wlo, llo + lld, @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
