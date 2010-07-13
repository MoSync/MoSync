#!/usr/bin/ruby

require 'FileUtils'
require 'settings.rb'
require 'skipped.rb'
require '../../rules/util.rb'

BUILD_DIR = 'build'
MOSYNCDIR = ENV['MOSYNCDIR']
GCC_FLAGS = " -I. -I#{MOSYNCDIR}/include -DNO_TRAMPOLINES"
PIPE_FLAGS = " -datasize=#{2*1024*1024} -stacksize=#{512*1024} -heapsize=#{1024*1024}"
PIPE_LIBS = " #{MOSYNCDIR}/lib/pipe_debug/mastd.lib"

# SETTINGS[:source_path] - directory in which source files are stored.

# use Work to build the tests?
# build them directly with command-line gcc & pipe-tool. no dependency or flags handling this way, but it would be faster.

# compile all tests

FileUtils.mkdir_p(BUILD_DIR)

sh "#{MOSYNCDIR}/bin/xgcc -g -S helpers/helpers.c -o build/helpers.s#{GCC_FLAGS}"

pattern = SETTINGS[:source_path] + '/*.c'
pattern.gsub!("\\", '/')
puts pattern
files = Dir.glob(pattern)
puts "#{files.count} files to test:"
files.each do |filename|
	bn = File.basename(filename)
	if(SKIPPED.include?(bn))
		puts "Skipped #{bn}"
		next
	end
	puts bn
	ofn = BUILD_DIR + '/' + bn.ext('.s')
	pfn = ofn.ext('.moo')
	winFile = ofn.ext('.win')
	failFile = ofn.ext('.fail')
	logFile = ofn.ext('.log')
	
	force_rebuild = SETTINGS[:rebuild_failed] && File.exists?(failFile)
	
	# build the program.
	if(!File.exists?(ofn) || force_rebuild)
		sh "#{MOSYNCDIR}/bin/xgcc -g -S \"#{filename}\" -o #{ofn}#{GCC_FLAGS}"
		force_rebuild = true
	end
	if(!File.exists?(pfn) || force_rebuild)
		sh "pipe-tool#{PIPE_FLAGS} -B #{pfn} #{ofn} build/helpers.s#{PIPE_LIBS}"
		force_rebuild = true
	end
	
	# execute it, if not win already, or we rebuilt something.
	
	if(File.exists?(winFile) && !force_rebuild)
		next
	end
	cmd = "#{MOSYNCDIR}/bin/more -noscreen -program #{pfn}"
	$stderr.puts cmd
	res = system(cmd)
	puts res
	if(res == true)	# success
		FileUtils.touch(winFile)
		FileUtils.rm_f(failFile)
		FileUtils.rm_f(logFile)
	else	# failure
		FileUtils.touch(failFile)
		FileUtils.rm_f(winFile)
		FileUtils.mv('log.txt', logFile)
		if(SETTINGS[:stop_on_fail])
			break
		end
	end
end
