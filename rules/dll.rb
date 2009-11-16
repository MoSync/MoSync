require "#{File.dirname(__FILE__)}/native_link.rb"

class DllTask < NativeGccLinkTask
	def initialize(work, name, objects, linkflags)
		super(work, name, objects)
		#" -Wl,-s -Wl,--enable-auto-import -Wl,-M -Wl,-x -Xlinker --no-undefined"
		@LINKFLAGS = " -shared" + linkflags
	end
	
	#def execute
		#libfile = File.dirname(@NAME) + "/lib" + File.basename(@NAME, ".dll") + ".a"
		#deffile = File.dirname(@NAME) + "/lib" + File.basename(@NAME, ".dll") + ".def"
		#sh "dlltool --export-all-symbols -z #{deffile} -l #{libfile} -D #{File.basename(@NAME)} #{@NAME}"
	#end
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
