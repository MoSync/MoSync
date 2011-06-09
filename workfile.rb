#!/usr/bin/ruby

# File.expand_path is used here to ensure the files are really only loaded once.
require File.expand_path('rules/targets.rb')
require File.expand_path('rules/host.rb')
require File.expand_path('rules/task.rb')
require File.expand_path('rules/mosync_util.rb')

PRE_DIRS = ["intlibs/idl-common", "intlibs/filelist"]

if(HOST == :win32) then
	INTLIB_PLATFORM = "windows"
	PLATFORM_TOOLS = ["tools/makesis-2.0.0", "tools/makesis-4", "tools/mifconv",
		"tools/rcomp", "tools/package", "tools/uidcrc", "tools/MoSyncUpdater"]
	ADDITIONAL_INTLIBS = ["intlibs/dgles-0.5"]
elsif(HOST == :darwin)
	INTLIB_PLATFORM = "linux"
	PLATFORM_TOOLS = ["tools/makesis-2.0.0_unix", "tools/makesis-4_unix",
		"tools/mifconv", "tools/rcomp", "tools/package", "tools/uidcrc"]
	ADDITIONAL_INTLIBS = []
else
	INTLIB_PLATFORM = HOST
	# todo: add lcab
	PLATFORM_TOOLS = []
	ADDITIONAL_INTLIBS = []
end

MORE_DIRS = ["intlibs/helpers/platforms/#{INTLIB_PLATFORM}",
	"intlibs/bluetooth",
	"intlibs/demangle",
	"intlibs/dll",
	"intlibs/gsm_amr",
	"intlibs/net",
	"intlibs/stabs",
	"runtimes/cpp/platforms/sdl",
	"runtimes/cpp/platforms/sdl/mosynclib",
	"runtimes/cpp/platforms/sdl/MoRE"
	]

BASE_DIRS = ADDITIONAL_INTLIBS + MORE_DIRS + PLATFORM_TOOLS

PIPE_DIRS = ["tools/protobuild", "tools/pipe-tool", "tools/DefaultSkinGenerator", "libs"]
EXAM_DIRS = ["tests/unitTest", "examples"]
TOOL_DIRS = ["tools/debugger", "tools/FontGenerator", "tools/PanicDoc", "tools/Bundle",
	"tests/unitTestServer", "tools/iphone-builder", "tools/icon-injector", "tools/e32hack"]

MAIN_DIRS = BASE_DIRS + TOOL_DIRS + PIPE_DIRS
ALL_DIRS = MAIN_DIRS + EXAM_DIRS

NEWLIB_DIRS = ["libs"]

class CopyDirWork < Work
	def initialize(name)
		@NAME = name
	end
	def setup
		builddir = "#{mosyncdir}/#{@NAME}"
		@prerequisites = [DirTask.new(self, builddir)]
		sources = Dir["#{@NAME}/*"]
		@prerequisites |= sources.collect do |src|
			CopyFileTask.new(self, FileTask.new(self, "#{builddir}/#{File.basename(src)}"),
				FileTask.new(self, src))
		end
	end
end

SKINS = CopyDirWork.new('skins')
RULES = CopyDirWork.new('rules')

target :base => [SKINS, RULES] do
	SKINS.invoke
	RULES.invoke
	Work.invoke_subdirs(PRE_DIRS)
	#Work.invoke_subdir("tools/WrapperGenerator", "compile")
	Work.invoke_subdir("tools/idl2", "compile")
end

target :default => :base do
	Work.invoke_subdirs(MAIN_DIRS)
end

target :examples => :base do
	Work.invoke_subdirs(PIPE_DIRS + EXAM_DIRS)
end

target :all => :default do
	Work.invoke_subdirs(EXAM_DIRS)
end

target :more => :base do
	Work.invoke_subdirs(MORE_DIRS)
end

target :newlib => :base do
	Work.invoke_subdirs(NEWLIB_DIRS)
end

target :libs => :base do
	Work.invoke_subdirs(PIPE_DIRS)
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
	Work.invoke_subdirs(ALL_DIRS, "clean")
end

target :all_configs do
	sh 'ruby workfile.rb all'
	sh 'ruby workfile.rb all CONFIG='
	sh 'ruby workfile.rb all USE_NEWLIB='
	sh 'ruby workfile.rb all USE_NEWLIB= CONFIG='
end

target :all_libs do
	sh 'ruby workfile.rb libs'
	sh 'ruby workfile.rb libs CONFIG='
	sh 'ruby workfile.rb libs USE_NEWLIB='
	sh 'ruby workfile.rb libs USE_NEWLIB= CONFIG='
end

Targets.invoke
