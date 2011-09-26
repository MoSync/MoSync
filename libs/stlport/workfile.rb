
#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

work = PipeLibWork.new
work.instance_eval do
	def collect_files(ending, ignoreSourceFiles = 'nil')
		default(:HEADER_DIRS, @SOURCES)
		files = []
		@HEADER_DIRS.each {|dir| files += Dir[dir+"/*"+ending]}
		files.flatten!
		if(defined?(@IGNORED_HEADERS))
			files.reject! {|file| @IGNORED_HEADERS.member?(File.basename(file)) }
		end
		files.reject! {|file| File.directory?(file) }
		return files.collect do |file| FileTask.new(self, file) end
	end

	def copyFiles(endings = [''])
		dir = mosync_include + "/" + @INSTALL_INCDIR
		# create a bunch of CopyFileTasks, then invoke them all.
		endings.each do |ending|
			collect_files(ending, '.c').each do |ending|
			task = CopyFileTask.new(self, dir + "/" + File.basename(ending.to_s), ending)
			@prerequisites = [task] + @prerequisites
		end
	end
	@prerequisites = [DirTask.new(self, dir)] + @prerequisites
	end

	def copyFilesCrtDir(name)
		@INSTALL_INCDIR = name
		@HEADER_DIRS = ["."]
		copyFiles()
	end

	def copyFilesSubDir(path, name)
		@INSTALL_INCDIR = path + "/" + name
		@HEADER_DIRS = [ name ]
		copyFiles()
	end

	def setup_pipe

		@SOURCES=["."]

		@EXTRA_INCLUDES = ["."]

		@EXTRA_CFLAGS = " -DUSE_EXOTIC_MATH -Wno-float-equal -Wno-unreachable-code -Wno-sign-compare -Wno-old-style-definition"
		if(CONFIG=="")
			# buggy compiler, buggy libs... I won't fix them.
			@EXTRA_CFLAGS += " -Wno-uninitialized"
		end
		if(CONFIG=="debug")
			@EXTRA_CFLAGS += " -DMOSYNCDEBUG"
		end

		copyFilesCrtDir("stlport")
		copyFilesSubDir("stlport", "stl")
		copyFilesSubDir("stlport", "stl/config")
		copyFilesSubDir("stlport", "stl/debug")
		copyFilesSubDir("stlport", "stl/pointers")
		copyFilesSubDir("stlport", "using")
		copyFilesSubDir("stlport", "using/h")

		if(USE_NEWLIB)
			@prerequisites << CopyFileTask.new(self, mosync_include + "/" + "newlib" + "/new",
				FileTask.new(self, "../libsupc++/new"))
			@prerequisites << CopyFileTask.new(self, mosync_include + "/" + "newlib" + "/macpp.h",
				FileTask.new(self, "../libsupc++/macpp.h"))
		end

		@HEADER_DIRS = ["."]

		@INSTALL_INCDIR = "stlport"

		@NAME = "stlport"
	end
end

work.invoke
