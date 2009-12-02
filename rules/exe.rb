require "#{File.dirname(__FILE__)}/native_link.rb"

class ExeTask < NativeGccLinkTask
	def initialize(work, name, objects, whole_libs, libs, linkflags)
		super(work, name, whole_libs + objects + libs)
		@FLAGS = linkflags
	end
end

# Compiles C/C++ code into an executable file.
# Supports GCC on linux and mingw
class ExeWork < NativeGccLinkWork
	def link_task_class
		ExeTask
	end
	def link_file_ending
		EXE_FILE_ENDING
	end
end
