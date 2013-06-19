#!/usr/bin/ruby

# File.expand_path is used here to ensure the files are really only loaded once.
require File.expand_path('rules/githooks.rb')
require File.expand_path('rules/targets.rb')
require File.expand_path('rules/host.rb')
require File.expand_path('rules/task.rb')
require File.expand_path('rules/mosync_util.rb')

enforceGithooks

PRE_DIRS = ["intlibs/idl-common", "intlibs/filelist"]

if(HOST == :win32) then
	INTLIB_PLATFORM = "windows"
	PLATFORM_TOOLS = ["tools/makesis-2.0.0", "tools/makesis-4",
		"tools/MoSyncUpdater"]
	ADDITIONAL_INTLIBS = ["intlibs/dgles-0.5", "intlibs/profiledb", "intlibs/mustache"]
elsif(HOST == :darwin)
	INTLIB_PLATFORM = "linux"
	PLATFORM_TOOLS = ["tools/makesis-2.0.0_unix", "tools/makesis-4_unix",
		]
	ADDITIONAL_INTLIBS = ["intlibs/profiledb", "intlibs/mustache"]
else
	INTLIB_PLATFORM = HOST
	# todo: add lcab
	PLATFORM_TOOLS = [
		'tools/MoCab',
	]
	ADDITIONAL_INTLIBS = ["intlibs/profiledb", "intlibs/mustache"]
end

MORE_DIRS = ["intlibs/helpers/platforms/#{INTLIB_PLATFORM}",
	"intlibs/bluetooth",
	"intlibs/demangle",
	"intlibs/dll",
	"intlibs/sqlite",
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
	"tests/unitTestServer", "tools/iphone-builder", "tools/icon-injector", "tools/e32hack",
	"tools/winphone-builder",
	"tools/mx-invoker",
	"tools/mx-config",
	"tools/profiledb", "tools/rescomp",
	"tools/mifconv", "tools/rcomp", "tools/package", "tools/uidcrc",
	"tools/nbuild"]

MAIN_DIRS = BASE_DIRS + TOOL_DIRS + PIPE_DIRS
ALL_DIRS = MAIN_DIRS + EXAM_DIRS

NEWLIB_DIRS = ["libs"]

SKINS = CopyDirWork.new(mosyncdir, 'skins')
RULES = CopyDirWork.new(mosyncdir, 'rules')

class ExtensionIncludeWork < Work
	def setup
		extIncDir = mosyncdir + '/ext-include'
		@prerequisites = []
		@prerequisites << DirTask.new(self, extIncDir)
		sources = [
			'runtimes/cpp/core/extensionCommon.h',
			'runtimes/cpp/core/ext/invoke-extension.h',
			'runtimes/cpp/core/ext/extension.h',
			'runtimes/cpp/core/syscall_arguments.h',
			'runtimes/cpp/core/CoreCommon.h',
			'intlibs/helpers/cpp_defs.h',
			'intlibs/helpers/maapi_defs.h',
			]
		sources.each do |src|
			@prerequisites << CopyFileTask.new(self, "#{extIncDir}/#{File.basename(src)}",
				FileTask.new(self, src))
		end
	end
end

EXTENSION_INCLUDES = ExtensionIncludeWork.new

target :base => [SKINS, RULES] do
	SKINS.invoke
	RULES.invoke
	Work.invoke_subdirs(PRE_DIRS)
	#Work.invoke_subdir("tools/WrapperGenerator", "compile")
	Work.invoke_subdir("tools/idl2", "compile")
	EXTENSION_INCLUDES.invoke
end

target :default => :base do
	Work.invoke_subdirs(MAIN_DIRS)
end

target :libs => :base do
	Work.invoke_subdirs(PIPE_DIRS)
end

target :examples => :libs do
	Work.invoke_subdirs_ex(true, EXAM_DIRS)
end

target :all => :default do
	Work.invoke_subdirs_ex(true, EXAM_DIRS)
end

target :more => :base do
	Work.invoke_subdirs(MORE_DIRS)
end

target :newlib => :base do
	Work.invoke_subdirs(NEWLIB_DIRS)
end

target :version do
	rev = open('|git rev-parse --verify HEAD').read.strip
	mod = open('|git status --porcelain').read.strip
	mod = 'mod ' if(mod.length > 0)
	fn = "#{mosyncdir}/bin/version.dat"
	open(fn, 'w') do |file|
		file.puts("Developer local build")
		file.puts(Time.new.strftime('%Y%m%d-%H%M'))
		file.puts(mod+rev)
	end
	puts "Wrote #{fn}:"
	puts open(fn).read.strip
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

target :clean_examples do
	Work.invoke_subdirs_ex(true, EXAM_DIRS, "clean")
end


def all_configs(target)
	sh "ruby workfile.rb #{target}"
	sh "ruby workfile.rb #{target} CONFIG="
	sh "ruby workfile.rb #{target} USE_NEWLIB="
	sh "ruby workfile.rb #{target} USE_NEWLIB= CONFIG="
end

target :all_configs do
	all_configs('all')
end

target :all_libs do
	all_configs('libs')
end

target :all_ex do
	all_configs('examples')
end

Targets.invoke
