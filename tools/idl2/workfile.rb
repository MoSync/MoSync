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

# let's pick a late target, to minimize the chances of something going wrong.
ct = FileTask.new(work, "../../runtimes/cpp/core/invoke_syscall_cpp.h")
ct.instance_eval do
	def setup
		@prerequisites = [@work.target] + (["extensions.h"] + Dir["*.idl"]).collect { |f|
			FileTask.new(@work, f)
		}
	end
	
	def execute
		sh @work.target
	end
end

target :compile => :default do
	#puts "IDL2 compile"
	#puts ct
	ct.setup
	#puts ct.inspect
	ct.invoke
end

target :clean do
	sh "rm -rf Output"
	#TODO: remove all copied files
end

Targets.invoke
