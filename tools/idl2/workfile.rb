#!/usr/bin/ruby

require File.expand_path('../../rules/exe.rb')
require File.expand_path('../../rules/targets.rb')

work = ExeWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["../../intlibs"]
	@LOCAL_LIBS = ["idl-common", "filelist"]
	@NAME = "idl2"
	@TARGETDIR = "."
end

target :default do
	work.invoke
end

# let's pick an early target, for great speed.
ct = FileTask.new(work, "Output/invoke_syscall_cpp.h")
ct.instance_eval do
	def setup
		@prerequisites = [DirTask.new(@work, '../../runtimes/java/Shared/generated')]
		@prerequisites += [@work.target] + (["extensions.h", "maapi_defs.h"] + Dir["*.idl"]).collect { |f|
			FileTask.new(@work, f)
		}
	end
	
	def execute
		sh @work.target
	end
end

target :compile => :default do
	begin
		ct.setup
		ct.invoke
	rescue => e
		# if the compiler should fail, we must make sure it runs next time.
		FileUtils.rm_f(ct.to_str)
		raise
	end
end

target :clean do
	verbose_rm_rf("build")
	verbose_rm_rf("Output")
	#TODO: remove all copied files
end

Targets.invoke
