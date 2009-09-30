BD = '.'
require 'build_rules/host.rb'
require 'build_rules/defaults.rb'

PRE_DIRS = ["intlibs/idl-common", "intlibs/filelist", "tools/GLWrapperGenerator"]

if(HOST == "win32") then
	INTLIB_PLATFORM = "windows"
	PLATFORM_TOOLS = ["tools/makesis-2.0.0", "tools/makesis-4"]
else
	INTLIB_PLATFORM = HOST
	PLATFORM_TOOLS = []
end

MAIN_DIRS = ["tools/protobuild",
	"tools/pipe-tool",
	"intlibs/helpers/platforms/#{INTLIB_PLATFORM}",
	"intlibs/bluetooth",
	"intlibs/gsm_amr",
	"intlibs/net",
	"intlibs/stabs",
	"runtimes/cpp/platforms/sdl",
	#"runtimes/cpp/platforms/sdl/mosynclib",
	"runtimes/cpp/platforms/sdl/MoRE",
	"tools/FontGenerator",
	"tools/PanicDoc"
	] + PLATFORM_TOOLS + ["libs", "examples"]

MORE_DIRS = ["intlibs/helpers/platforms/#{INTLIB_PLATFORM}",
	"intlibs/bluetooth",
	"intlibs/gsm_amr",
	"intlibs/net",
	"intlibs/stabs",
	"runtimes/cpp/platforms/sdl",
	"runtimes/cpp/platforms/sdl/mosynclib",
	"runtimes/cpp/platforms/sdl/MoRE"
	] + PLATFORM_TOOLS

EXAM_DIRS = ["tools/protobuild",
	"tools/pipe-tool"
	] + PLATFORM_TOOLS + ["libs", "examples"]

def do_subdir(dir, target="")
	sh "cd #{dir} && rake -N #{target} CONFIG=\"#{CONFIG}\""
end

def do_subdirs(dirs, target="")
	dirs.each do |dir| do_subdir(dir, target) end
end

SKIN_TARGET_DIR = "#{ENV['MOSYNCDIR']}/skins"
directory SKIN_TARGET_DIR
task :skins => SKIN_TARGET_DIR

SKIN_SOURCES = FileList.new('skins/*')

SKIN_SOURCES.each do |path|
	target = "#{SKIN_TARGET_DIR}/#{File.basename(path)}"
	file target => path do |t|
		sh "cp #{path} #{target}"
	end
	task :skins => target
end

task :base => :skins do
	do_subdirs(PRE_DIRS)
	do_subdir("tools/idl2", "compile")
end

task :default => :base do
	do_subdirs(MAIN_DIRS)
end

task :more => :base do
	do_subdirs(MORE_DIRS)
end

task :examples => :base do
	do_subdirs(EXAM_DIRS)
end

task :clean do
	do_subdirs(PRE_DIRS, "clean")
	do_subdir("tools/idl2", "clean")
	do_subdirs(MAIN_DIRS, "clean")
end
