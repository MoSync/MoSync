# File.expand_path is used here to ensure the files are really only loaded once.
require File.expand_path('../../rules/task.rb')

work = Work.new
work.instance_eval do
	def setup
		@TARGET_FILES = ["../idl2/opengl_generated.idl",
			"../../runtimes/java/Shared/generated/opengl_invoke_generated.jpp",
			"../../libs/MAStd/GLES/magl_functions.h"]
		
		genTask = Task.new(self)
		genTask.instance_eval do
			def execute
				sh "ruby GLWrapperGen.rb"
			end
		end
		
		@prerequisites = @TARGET_FILES.collect { |t|
			ft = FileTask.new(self, File.basename(t))
			ft.instance_eval do
				@prerequisites = [genTask]
			end
			CopyFileTask.new(self, t, ft)
		}
	end
end

work.invoke
