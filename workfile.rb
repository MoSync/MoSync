#!/usr/bin/ruby

# File.expand_path is used here to ensure the files are really only loaded once.
require File.expand_path('rules/targets.rb')
require File.expand_path('rules/host.rb')
require File.expand_path('rules/task.rb')

PRE_DIRS = ["intlibs/idl-common", "intlibs/filelist", "tools/GLWrapperGenerator"]

if(HOST == :win32) then
	INTLIB_PLATFORM = "windows"
	PLATFORM_TOOLS = ["tools/makesis-2.0.0", "tools/makesis-4"]
else
	INTLIB_PLATFORM = HOST
	PLATFORM_TOOLS = []
end

MORE_DIRS = ["intlibs/helpers/platforms/#{INTLIB_PLATFORM}",
	"intlibs/bluetooth",
	"intlibs/gsm_amr",
	"intlibs/net",
	"intlibs/stabs",
	"runtimes/cpp/platforms/sdl",
	"runtimes/cpp/platforms/sdl/mosynclib",
	"runtimes/cpp/platforms/sdl/MoRE"
	]

BASE_DIRS = MORE_DIRS + PLATFORM_TOOLS

EXAM_DIRS = ["tools/protobuild", "tools/pipe-tool", "tools/e32hack", "libs", "tests/unitTest", "examples"]

MAIN_DIRS = BASE_DIRS + ["tools/FontGenerator", "tools/PanicDoc"] + EXAM_DIRS

NEWLIB_DIRS = ["libs", "tests/unitTest", "examples"]

skins = Work.new
skins.instance_eval do
	def setup
		builddir = "#{ENV['MOSYNCDIR']}/skins"
		@prerequisites = [DirTask.new(self, builddir)]
		skin_sources = Dir['skins/*']
		@prerequisites |= skin_sources.collect do |src|
			CopyFileTask.new(self, FileTask.new(self, "#{builddir}/#{File.basename(src)}"),
				FileTask.new(self, src))
		end
	end
end

target :base => skins do
	Work.invoke_subdirs(PRE_DIRS)
	Work.invoke_subdir("tools/idl2", "compile")
end

target :default => :base do
	Work.invoke_subdirs(MAIN_DIRS)
end

target :noidl => skins do
	Work.invoke_subdirs(PRE_DIRS)
	Work.invoke_subdir("tools/idl2")
	Work.invoke_subdirs(MAIN_DIRS)
end

target :more => :base do
	Work.invoke_subdirs(MORE_DIRS)
end

target :examples => :base do
	Work.invoke_subdirs(EXAM_DIRS)
end

target :newlib => :base do
	Work.invoke_subdirs(NEWLIB_DIRS)
end

target :clean_more do
	verbose_rm_rf("build")
	Work.invoke_subdirs(PRE_DIRS, "clean")
	Work.invoke_subdir("tools/idl2", "clean")
	Work.invoke_subdirs(MORE_DIRS, "clean")
end

target :clean do
	verbose_rm_rf("build")
	Work.invoke_subdirs(PRE_DIRS, "clean")
	Work.invoke_subdir("tools/idl2", "clean")
	Work.invoke_subdirs(MAIN_DIRS, "clean")
end

Targets.invoke
