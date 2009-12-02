require "#{File.dirname(__FILE__)}/gcc.rb"
#require 'lib/importenv.rb'

class PipeTask < FileTask
	def initialize(work, name, objects, linkflags)
		super(work, name)
		@FLAGS = linkflags
		dirTask = DirTask.new(work, File.dirname(name))
		@objects = objects
		@prerequisites = @objects + [dirTask]
	end
	def execute
		sh "#{ENV["MOSYNCDIR"]}/bin/pipe-tool#{@FLAGS} #{@NAME} #{@objects.join(' ')}"
		if(!File.exist?(@NAME))
			error "Pipe-tool failed silently!"
		end
	end
end

class PipeGccWork < GccWork
	def gcc; ENV["MOSYNCDIR"] + "/bin/xgcc"; end
	def gccmode; "-S"; end
	
	def set_defaults
		@BUILDDIR_BASE = "build/pipe/"
		super
	end
	
	private
	
	def object_ending; ".s"; end
	
	def setup3(all_objects)
		#puts all_objects
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".lib") }
		need(:@NAME)
		need(:@BUILDDIR)
		need(:@TARGETDIR)
		target = @TARGETDIR + "/" + @BUILDDIR + filename
		@TARGET = PipeTask.new(self, target, all_objects + llo, @FLAGS + @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
