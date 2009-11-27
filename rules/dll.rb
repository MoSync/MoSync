require "#{File.dirname(__FILE__)}/native_link.rb"

class DllTask < NativeGccLinkTask
	def initialize(work, name, objects, whole_libs, libs, linkflags)
		super(work, name, objects)
		@prerequisites += whole_libs + libs
		libfile = File.dirname(@NAME) + "/lib" + File.basename(@NAME, ".dll") + ".a"
		libflags = ""
		if(whole_libs.size > 0)
			libflags += " -Wl,--whole-archive #{whole_libs.join(' ')} -Wl,--no-whole-archive"
		end
		if(libs.size > 0)
			libflags += ' ' + libs.join(' ')
		end
		if(CONFIG == "")
			libflags += " -s"
		end
		@LINKFLAGS = libflags + " -shared -Wl,--out-implib=#{libfile}" + linkflags
	end
end

# Compiles C/C++ code into an dynamic-link library.
# Supports GCC on linux and mingw.
class DllWork < NativeGccLinkWork
	def link_task_class
		DllTask
	end
	def link_file_ending
		DLL_FILE_ENDING
	end
end
