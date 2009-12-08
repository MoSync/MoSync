require "#{File.dirname(__FILE__)}/host.rb"
require "#{File.dirname(__FILE__)}/task.rb"
require "#{File.dirname(__FILE__)}/gcc_flags.rb"
require "#{File.dirname(__FILE__)}/loader_md.rb"
require "#{File.dirname(__FILE__)}/flags.rb"

# Compiles a source file using gcc.
# Generates extra files for tracking dependencies and flags,
# so that if the flags or any dependency have changed, this file will be recompiled.
# Objects of this class are created by GccWork.
class CompileGccTask < FileTask
	def initialize(work, name, source, cflags)
		super(work, name)
		@SOURCE = source
		@prerequisites = [source]
		
		@DEPFILE = @work.genfile(source, ".mf")
		depFlags = " -MMD -MF #{@DEPFILE}"
		@FLAGS = cflags + depFlags
		
		initFlags
		
		# only if the file is not already needed do we care about extra dependencies
		if(!needed?(false)) then
			@prerequisites = MakeDependLoader.load(@DEPFILE, @NAME)
		end
	end
	
	def needed?(log = true)
		return true if(super(log))
		if(!File.exists?(@DEPFILE))
			puts "Because the dependency file is missing:" if(log)
			return true
		end
		return flagsNeeded?(log)
	end
	
	def execute
		execFlags
		sh "#{@work.gcc} -o #{@NAME}#{@FLAGS} #{@work.gccmode} #{@SOURCE}"
	end
	
	include FlagsChanged
end

# Base class.
# Compiles C/C++ code into an executable file.
# Supports GCC on mingw, pipe and linux.
# Uses the following variables: @SOURCES, @IGNORED_FILES, @EXTRA_SOURCEFILES,
# @SPECIFIC_CFLAGS and @EXTRA_OBJECTS.
class GccWork < BuildWork
	# Returns a path representing a generated file, given a source filename and a new file ending.
	def genfile(source, ending)
		@BUILDDIR + File.basename(source.to_s).ext(ending)
	end
	
	# The filename of the target.
	def target
		@TARGET
	end
	
	private
	
	include GccFlags
	
	def setup2
		define_cflags
		@CFLAGS_MAP = { ".c" => @CFLAGS,
			".cpp" => @CPPFLAGS,
			".cc" => @CPPFLAGS }
		
		#find source files
		cfiles = collect_files(".c")
		cppfiles = collect_files(".cpp") + collect_files(".cc") 
		@all_sourcefiles = cfiles + cppfiles
		
		@source_objects = objects(@all_sourcefiles)
		all_objects = @source_objects + @EXTRA_OBJECTS
		
		setup3(all_objects)
 	end
	
	# returns an array of FileTasks
	def collect_files(ending)
		files = @SOURCES.collect {|dir| Dir[dir+"/*"+ending]}
		files.flatten!
		files.reject! {|file| @IGNORED_FILES.member?(File.basename(file))}
		files += @EXTRA_SOURCEFILES.select do |file| file.getExt == ending end
		return files.collect do |file| FileTask.new(self, file) end
	end
	
	def getGccFlags(source)
		ext = source.to_s.getExt
		cflags = @CFLAGS_MAP[ext]
		if(cflags == nil) then
			error "Bad ext: '#{ext}' from source '#{source}'"
		end
		need(:@SPECIFIC_CFLAGS)
		cflags += @SPECIFIC_CFLAGS.fetch(File.basename(source.to_s), "")
		return cflags
	end
	
	def makeGccTask(source, ending)
		objName = genfile(source, ending)
		task = CompileGccTask.new(self, objName, source, getGccFlags(source))
		return task
	end
	
	# returns an array of CompileGccTasks
	def objects(sources)
		return sources.collect do |path| makeGccTask(path, object_ending) end
	end
end
