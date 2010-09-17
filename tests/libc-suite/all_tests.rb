#!/usr/bin/ruby

require 'FileUtils'
require 'settings.rb'
require 'skipped.rb'
require '../../rules/util.rb'

if(ARGV.length > 0)
	SETTINGS[:stop_on_fail] = true
	SETTINGS[:rebuild_failed] = true
	SETTINGS[:retry_failed] = true
end

BUILD_DIR = 'build'
MOSYNCDIR = ENV['MOSYNCDIR']
GCC_FLAGS = " -I- -std=gnu99 -I. -Isys -I#{MOSYNCDIR}/include/newlib -I \"#{SETTINGS[:source_path][0..-2]}\" -DNO_TRAMPOLINES -DUSE_EXOTIC_MATH -include skeleton.h"
PIPE_FLAGS = " -datasize=#{2*1024*1024} -stacksize=#{512*1024} -heapsize=#{1024*1024}"
PIPE_LIBS = " build/helpers.s #{MOSYNCDIR}/lib/newlib_debug/newlib.lib"

# SETTINGS[:source_path] - directory in which source files are stored.

# use Work to build the tests?
# build them directly with command-line gcc & pipe-tool. no dependency or flags handling this way,
# but it would be faster.

FileUtils.mkdir_p(BUILD_DIR)
FileUtils.rm_rf('filesystem')
FileUtils.mkdir_p('filesystem/tmp')

sh "#{MOSYNCDIR}/bin/xgcc -g -Werror -S #{File.expand_path('helpers.c')} -o build/helpers.s#{GCC_FLAGS}"

# Find tests.
# We have many directories. Some of these have Makefiles with a definition of a "tests" variable.
# This variable is a list of names of test programs, without the file ending, which, in this case,
# is invariably '.c'.

# Go through each directory, search for a Makefile and parse it to find "tests". Print the results.

MAKEFILE_TEST_ARRAYS = ['tests', 'tests-static', 'libm-tests', 'strop-tests']

def process_line(line)
	lineIsInteresting = false
	MAKEFILE_TEST_ARRAYS.each do |ta|
		if(line.beginsWith(ta))
			lineIsInteresting = true
		end
	end
	if(lineIsInteresting)
		#p line
		words = line.scan(/[^ \t]+/)
		
		#puts "test: #{words[0]}"
		return nil unless(MAKEFILE_TEST_ARRAYS.include?(words[0]))
		#p words
		if(words[1] == '=' || words[1] == ':=' || words[1] == '+=')
			result = words.slice(2..-1)
			if(words[0] == 'strop-tests')
				#p result
				result.collect! do |t| "test-#{t}" end
				#p result
			end
			return result
		end
	end
	return nil
end

SPACE_MARK = "__&NBSP;__"
def parse_makefile(filename)
	tests = []
	open(filename) do |mf|
		# Read the entire file.
		lines = mf.read
		
		# Get rid of comments, and combine logical lines into physical lines, for easy processing.
		lines.gsub!(/\\ /, SPACE_MARK)
		lines.gsub!(/#[^\n]*\n/m, "")
		lines.gsub!(/\\\n/, ' ')
		lines.split("\n").each do |line|
			new_tests = process_line(line)
			#p new_tests
			tests += new_tests if(new_tests)
		end
	end
	return tests
end

pattern = "#{SETTINGS[:source_path]}*/"
p pattern
dirs = Dir[pattern]
total = 0
files = []
#p dirs
dirs.each do |dir|
	dirName = File.basename(dir)
	if(SKIPPED_DIRECTORIES.include?(dirName))
		puts "Skipped #{dirName}"
		next
	end
	mfPath = dir + 'Makefile'
	if(File.exists?(mfPath))
		$stdout.write dirName + ': '
		tests = parse_makefile(mfPath)
		if(tests)
			puts tests.size
			total += tests.size
			#p tests
			tests.each do |t|
				files << dir + t + '.c'
			end
		end
	else
		puts dirName + ' has no Makefile.'
	end
end

puts
puts "Total: #{total}"

# compile all tests

def delete_if_empty(filename)
	if(!File.size?(filename))
		if(File.exists?(filename))
			FileUtils.rm(filename)
		end
	end
end

def link_and_test(ofn, dead_code, force_rebuild)
	suffix = dead_code ? 'e' : ''
	pfn = ofn.ext('.moo' + suffix)
	winFile = ofn.ext('.win' + suffix)
	failFile = ofn.ext('.fail' + suffix)
	logFile = ofn.ext('.log' + suffix)
	mdsFile = ofn.ext('.md.s')
	esFile = ofn.ext('.e.s')
	sldFile = ofn.ext('.sld' + suffix)
	stabsFile = ofn.ext('.stabs' + suffix)
	
	delete_if_empty(pfn)
	
	# link
	if(!File.exists?(pfn) || force_rebuild)
		if(dead_code)
			sh "pipe-tool#{PIPE_FLAGS} -elim -master-dump -B #{pfn} #{ofn} #{PIPE_LIBS}"
			sh "pipe-tool -sld=#{sldFile} -B #{pfn} rebuild.s"
		else
			sh "pipe-tool -sld=#{sldFile} -stabs=#{stabsFile}#{PIPE_FLAGS} -B #{pfn} #{ofn} #{PIPE_LIBS}"
		end
		force_rebuild = true
	end
	delete_if_empty(pfn)
	if(!File.exists?(pfn))
		error"Unknown link failure."
	end
	
	# execute it, if not win already, or we rebuilt something.
	
	if((File.exists?(winFile) || !SETTINGS[:retry_failed]) && !force_rebuild)
		return force_rebuild
	end
	cmd = "#{MOSYNCDIR}/bin/more -timeout 20 -allowdivzero -noscreen -program #{pfn} -sld #{sldFile}"
	$stderr.puts cmd
	res = system(cmd)
	puts res
	if(res == true)	# success
		FileUtils.touch(winFile)
		FileUtils.rm_f(failFile)
		FileUtils.rm_f(logFile)
		FileUtils.rm_f(mdsFile)
		FileUtils.rm_f(esFile)
		FileUtils.rm_f(sldFile)
		FileUtils.rm_f(stabsFile)
	else	# failure
		FileUtils.touch(failFile)
		FileUtils.rm_f(winFile)
		FileUtils.mv('log.txt', logFile) if(File.exists?('log.txt'))
		FileUtils.mv('_masterdump.s', mdsFile) if(File.exists?('_masterdump.s'))
		FileUtils.mv('rebuild.s', esFile) if(File.exists?('rebuild.s'))
		if(SETTINGS[:stop_on_fail])
			error "Stop on fail"
		end
	end
	return force_rebuild
end

#puts "premature exit"
#exit 0

if(ARGV.size > 0)
	files.reject! do |f| !ARGV.include?(File.basename(f)) end
end

unskippedCount = 0

files.each do |filename|
	bn = File.basename(filename)
	if(!File.exists?(filename))
		puts "Nonexistant: #{bn}"
		next
	end
	skip = SKIPPED_FILES.include?(bn)
	SKIPPED_PATTERNS.each do |r|
		if(r.match(bn))
			skip = true
			break
		end
	end
	if(skip)
		puts "Skipped #{bn}"
		next
	end
	puts bn
	unskippedCount += 1
	
	ofn = BUILD_DIR + '/' + bn.ext('.s')
	force_rebuild |= SETTINGS[:rebuild_failed] && (File.exists?(ofn.ext('.fail')) || File.exists?(ofn.ext('.faile')))
	force_rebuild |= SETTINGS[:rebuild_missing_log] && !File.exists?(ofn.ext('.win')) && !File.exists?(ofn.ext('.log'))
	force_rebuild |= !File.exists?(ofn.ext('.win')) && !File.exists?(ofn.ext('.fail'))
	
	# compile
	if(!File.exists?(ofn) || force_rebuild)
		sh "#{MOSYNCDIR}/bin/xgcc -g -S \"#{filename}\" -o #{ofn}#{GCC_FLAGS} -I \"#{File.dirname(filename)}\""
		force_rebuild = true
	end
	
	force_rebuild = link_and_test(ofn, false, force_rebuild)
	if(:test_dead_code_elimination && File.exists?(ofn.ext('.win')))
		link_and_test(ofn, true, force_rebuild)
	end
end

puts "#{unskippedCount} actual tests."
