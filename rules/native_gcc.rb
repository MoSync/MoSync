require "#{File.dirname(__FILE__)}/host.rb"
require "#{File.dirname(__FILE__)}/task.rb"
require "#{File.dirname(__FILE__)}/gcc_flags.rb"
require "#{File.dirname(__FILE__)}/loader_md.rb"

class CompileGccTask < FileTask
		
	def initialize(work, name, source, cflags)
		super(work, name)
		@SOURCE = source
		@prerequisites = [source]
		
		@DEPFILE = @work.genfile(source, ".mf")
		depFlags = " -MMD -MF #{@DEPFILE}"
		
		# only if the file is not already needed do we care about extra dependencies
		if(!needed?) then
			@prerequisites = MakeDependLoader.load(@DEPFILE, @NAME)
		end
		
		@CFLAGS = cflags + depFlags

		# save cflags to disk, for use as dependency.
		# but first read it back from disk, if existing.
		@FLAGSFILE = @work.genfile(source, ".flags")
		if(File.exists?(@FLAGSFILE)) then
			@OLDFLAGS = open(@FLAGSFILE) { |f| f.read }
		end
	end
	
	def needed?
		if(@OLDFLAGS != @CFLAGS)
			puts "Because the flags have changed:"
			return true
		end
		if(!File.exists?(@DEPFILE))
			puts "Because the dependency file is missing:"
			return true
		end
		super
	end
	
	def execute
		if(@OLDFLAGS != @CFLAGS) then
			open(@FLAGSFILE, 'w') { |f| f.write(@CFLAGS) }
		end
		sh "gcc -o #{@NAME}#{@CFLAGS} -c #{@SOURCE}"
	end
end

# Compiles C/C++ code into an executable file.
# Supports GCC on linux and mingw
class NativeGccWork < BuildWork
	def genfile(source, ending)
		@BUILDDIR + File.basename(source.to_s).ext(ending)
	end
	
	def target
		@TARGET
	end
	
	private
	
	include GccFlags
	
	def setup2
		#puts "NativeGccWork.setup2: #{@NAME.inspect}"
		define_cflags
		@CFLAGS_MAP = { ".c" => @CFLAGS,
			".cpp" => @CPPFLAGS,
			".cc" => @CPPFLAGS }

		#find source files
		cfiles = collect_files(".c")
		cppfiles = collect_files(".cpp") + collect_files(".cc") 
		@all_sourcefiles = cfiles + cppfiles

		@source_objects = objects(@all_sourcefiles)
		#puts "ho"
		#puts @EXTRA_OBJECTS
		all_objects = @source_objects + @EXTRA_OBJECTS
		
		setup3(all_objects)
 	end
	
	# returns an array of Tasks
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
		#puts "Ext: '#{ext}' from source '#{source}'"
		if(cflags == nil) then
			error "Bad ext: '#{ext}' from source '#{source}'"
		end
		#puts "got flags: #{cflags}"
		cflags += @SPECIFIC_CFLAGS.fetch(File.basename(source.to_s), "")
		return cflags
	end
	
	def makeGccTask(source, ending)
		objName = genfile(source, ending)
		task = CompileGccTask.new(self, objName, source, getGccFlags(source))
		return task
	end
	
	# returns an array of Tasks
	def objects(sources)
		return sources.collect do |path| makeGccTask(path, ".o") end
	end
end
