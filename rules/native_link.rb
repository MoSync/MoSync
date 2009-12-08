require "#{File.dirname(__FILE__)}/native_gcc.rb"
require "#{File.dirname(__FILE__)}/flags.rb"

# Base class.
# Links object files together to form an native executable or shared library.
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
		#puts "objects: #{@objects.join(' ')}"
		#puts "flags: #{@FLAGS}"
		sh "g++ #{@objects.join(' ')}#{@FLAGS} -o #{@NAME}"
	end
	
	include FlagsChanged
end

# Base class.
# Compiles C/C++ code into an native executable file or shared library.
# Supports GCC on mingw and linux.
# In addition to the variables used by GccWork, this class uses the following:
# @NAME, @LOCAL_LIBS, @LOCAL_DLLS, @WHOLE_LIBS, @LIBRARIES, @COMMON_BUILDDIR, @BUILDDIR and @TARGETDIR.
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
		#puts "@EXTRA_LINKFLAGS: "+@EXTRA_LINKFLAGS
		@TARGET = link_task_class.new(self, target, all_objects, wlo, llo + lld, @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
