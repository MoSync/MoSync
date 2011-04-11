#!/usr/bin/ruby

require 'FileUtils'
require './settings.rb'
require './skipped.rb'
require '../../rules/util.rb'
require '../../rules/host.rb'

if(ARGV.length > 0)
	SETTINGS[:stop_on_fail] = true
	SETTINGS[:rebuild_failed] = true
	SETTINGS[:retry_failed] = true
end

OPT_VERSION = SETTINGS[:test_release] ? 'release' : 'debug'
OPT_FLAGS = SETTINGS[:test_release] ? ' -O2' : ''
BUILD_DIR = 'build/' + OPT_VERSION
MOSYNCDIR = ENV['MOSYNCDIR']
GCC_FLAGS = " -I- -std=gnu99 -I. -Isys -I#{MOSYNCDIR}/include/newlib -I \"#{SETTINGS[:source_path][0..-2]}\""+
	" -DNO_TRAMPOLINES -DUSE_EXOTIC_MATH -include skeleton.h #{OPT_FLAGS}"
PIPE_FLAGS = " -datasize=#{12*1024*1024} -stacksize=#{512*1024} -heapsize=#{1024*1024*10}"
PIPE_LIBS = " build/helpers.s #{MOSYNCDIR}/lib/newlib_#{OPT_VERSION}/newlib.lib"

require './argv.rb'

# SETTINGS[:source_path] - directory in which source files are stored.

# use Work to build the tests?
# build them directly with command-line gcc & pipe-tool. no dependency or flags handling this way,
# but it would be faster.

FileUtils.mkdir_p(BUILD_DIR)
FileUtils.rm_rf('filesystem')
FileUtils.mkdir_p('filesystem/tmp')

def input_files(filename)
	base = File.dirname(filename) + '/' + File.basename(filename, File.extname(filename))
	inputName = base + '.input'
	if(File.exists?(inputName))
		return [inputName]
	else
		return []
	end
end

def writeArgvFile(filename, argv, testSrcName)
	argv = DEFAULT_ARGV if(!argv)
	file = open(filename, 'w')
	file.write("const char* gArgv[] = {\"#{File.basename(testSrcName, File.extname(testSrcName))}\", ")
	argv.each do |arg|
		file.write("\"#{arg}\",")
	end
	file.write(" 0 };\n")
	file.write("const int gArgc = #{argv.size + 1};\n")
	inputs = input_files(testSrcName)
	file.write("\n")
	file.write("#include <stdio.h>\n") if(!inputs.empty?)
	#file.write("#include <stdlib.h>\n") if(!inputs.empty?)
	file.write("void setup_stdin() {\n")
	if(!inputs.empty?)
		file.write("\tstdin = fopen(\"#{File.basename(inputs[0])}\", \"r\");\n")
	end
	file.write("}\n")
	file.close
end

def doArgv(baseName, argv, testSrcName, force_rebuild)
	return if(!force_rebuild)
	cName = "build/argv-#{baseName}.c"
	sName = "build/argv-#{baseName}.s"
	writeArgvFile(cName, argv, testSrcName)
	sh "#{MOSYNCDIR}/bin/xgcc -g -I#{MOSYNCDIR}/include/newlib -Werror -S #{File.expand_path(cName)} -o #{sName}"
	return sName
end

DEFAULT_ARGV_SFILE = doArgv('default', DEFAULT_ARGV, 'default', true)


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
files = {}
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
			tests.uniq!
			puts tests.size
			total += tests.size
			#p tests
			tests.each do |t|
				fn = t + '.c'
				files[dir + fn] = dirName
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

def link_and_test(ofn, argvs, files, dead_code, force_rebuild, inputs, code)
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
			sh "pipe-tool#{PIPE_FLAGS} -elim -master-dump -B #{pfn} #{ofn} #{argvs} #{PIPE_LIBS}"
			sh "pipe-tool -sld=#{sldFile} -B #{pfn} rebuild.s"
		else
			sh "pipe-tool -master-dump -sld=#{sldFile} -stabs=#{stabsFile}#{PIPE_FLAGS} -B #{pfn} #{ofn} #{argvs} #{PIPE_LIBS}"
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
	
	# copy files only when executing
	files.each do |file|
		FileUtils.cp_r(file, 'filesystem/')
	end
	inputs.each do |input|
		sh "dos2unix --d2u \"filesystem/#{File.basename(input)}\""
	end
	
	if(code)
		code.call
	end
	
	cmd = "#{MOSYNCDIR}/bin/more -timeout 600 -allowdivzero -noscreen -program #{pfn} -sld #{sldFile}"
	if(HOST == :win32)
		cmd = "start /B /BELOWNORMAL /WAIT #{cmd}"
	end
	$stderr.puts cmd
	startTime = Time.now
	res = system(cmd)
	endTime = Time.now
	puts res
	puts "Elapsed time: #{endTime - startTime}"
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
			if(SETTINGS[:copy_targets])
				SETTINGS[:copy_targets].each do |target|
					# copy program, sld and stabs to directory :copy_target.
					FileUtils.cp(pfn, target + 'program')
					FileUtils.cp(sldFile, target + 'sld.tab')
					FileUtils.cp(stabsFile, target + 'stabs.tab')
				end
			end
			error "Stop on fail"
		end
	end
	return force_rebuild
end

#puts "premature exit"
#exit 0

# check for dupes
basenames = {}
overrides = []
files.each do |f, dir|
	bn = File.basename(f)
	if(basenames[bn])
		puts "Duplicate: #{f} - #{basenames[bn]}"
		overrides << f
		overrides << basenames[bn]
	else
		basenames[bn] = f
	end
end

# mark dupes
new_files = {}
files.each do |f, dir|
	bn = File.basename(f)
	if(overrides.include?(f))
		new_files[f] = dir + '_' + bn
		puts "de-dupe: #{new_files[f]}"
	else
		new_files[f] = bn
	end
end
files = new_files

if(ARGV.size > 0)
	files.reject! do |f, bn| !ARGV.include?(bn) end
end


unskippedCount = 0

files.each do |filename, targetName|
	bn = targetName
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
	
	inputs = input_files(filename)
	
	argv = SPECIFIC_ARGV.fetch(bn, nil)
	if(argv != nil || !inputs.empty?)
		argvs = doArgv(bn, argv, filename, force_rebuild)
	else
		argvs = DEFAULT_ARGV_SFILE
	end
	
	files = SPECIFIC_FILES.fetch(bn, [])
	files += inputs
	
	code = SPECIFIC_CODE.fetch(bn, nil)
	
	force_rebuild = link_and_test(ofn, argvs, files, false, force_rebuild, inputs, code)
	if(SETTINGS[:test_dead_code_elimination] && File.exists?(ofn.ext('.win')))
		link_and_test(ofn, argvs, files, true, force_rebuild, inputs, code)
	end
end

puts "#{unskippedCount} actual tests."
