# File.expand_path is used here to ensure the files are really only loaded once.
require File.expand_path('../../rules/task.rb')

work = Work.new
work.instance_eval do
	class GenTask < FileTask
		def execute
			sh "ruby GLWrapperGen.rb"
		end
	end
	
	def setup
		@TARGET_FILES = ["../idl2/opengl_generated.idl",
			"../../runtimes/java/Shared/generated/opengl_invoke_generated.jpp",
			"../../libs/MAStd/GLES/magl_functions.h"]
		
		@prerequisites = @TARGET_FILES.collect { |t|
			gt = GenTask.new(self, File.basename(t))
			CopyFileTask.new(self, t, gt)
		}
	end
end

work.invoke
