#!/usr/bin/ruby

require 'fileutils'
require './settings.rb'
require './skipped.rb'
require '../../rules/util.rb'
require '../../rules/host.rb'

if(ARGV.length > 0)
	SETTINGS[:stop_on_fail] = true
	SETTINGS[:rebuild_failed] = true
	SETTINGS[:retry_failed] = true
	SETTINGS[:keep_parts] = true
end

OPT_VERSION = SETTINGS[:test_release] ? 'release' : 'debug'
OPT_FLAGS = SETTINGS[:test_release] ? ' -O2' : ''
BUILD_DIR = 'build/' + OPT_VERSION
MOSYNCDIR = ENV['MOSYNCDIR']
GCC_FLAGS = " -I- -std=gnu99 -I. -Isys -I#{MOSYNCDIR}/include/newlib -I \"#{SETTINGS[:source_path][0..-2]}\""+
	" -DNO_TRAMPOLINES -DUSE_EXOTIC_MATH -include skeleton.h #{OPT_FLAGS}"+
	' -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wshadow -Wpointer-arith -Wundef -Wfloat-equal'+
	' -Winit-self -Wmissing-noreturn -Wmissing-format-attribute'

PIPE_FLAGS = " -datasize=#{12*1024*1024} -stacksize=#{512*1024} -heapsize=#{1024*1024*10}"
PIPE_LIBS = " build/helpers.s build/setup_filesystem.s #{MOSYNCDIR}/lib/newlib_#{OPT_VERSION}/newlib.lib"

require './argv.rb'

# SETTINGS[:source_path] - directory in which source files are stored.

# use Work to build the tests?
# build them directly with command-line gcc & pipe-tool. no dependency or flags handling this way,
# but it would be faster.

FileUtils.mkdir_p(BUILD_DIR)

def clear_filesystem
	FileUtils.rm_rf('filesystem')
	FileUtils.mkdir_p('filesystem/tmp')
end
clear_filesystem

# treat filesystem/ as root. output a compiled resource file that setup_filesystem() can use
# to reproduce all the files.
def writeResourceFile(name)
	resFileName = "#{name}.lst"
	resFile = open("#{resFileName}", 'w')
	resFile.puts('.res')
	resFile.puts('.label "start"')
	
	dir = Dir.new('filesystem/')
	doResourceDir(resFile, dir, '/', 0)
	
	resFile.puts('')
	resFile.puts('.res')
	resFile.puts('.label "end"')
	resFile.close
	
	FileUtils.cd 'build'
	sh "#{MOSYNCDIR}/bin/pipe-tool -R resources ../#{resFileName}"
	FileUtils.cd '..'
end

def doResourceDir(resFile, dir, prefix, count)
	dir.each do |name|
		next if(name[0,1] == '.')
		realPath = dir.path+name
		resFile.puts('')
		resFile.puts(".res RES_FILE_#{count}")
		count += 1
		resFile.puts(".bin")
		if(File.directory?(realPath))
			runtimeName = prefix+name+'/'
			resFile.puts(".cstring \"#{runtimeName}\"")
			
			d2 = Dir.new(realPath+'/')
			count = doResourceDir(resFile, d2, runtimeName, count)
		else
			runtimeName = prefix+name
			resFile.puts(".cstring \"#{runtimeName}\"")
			resFile.puts(".include \"../../#{realPath}\"") if(File.size(realPath) > 0)
		end
	end
	return count
end

# output a default resource file
DEFAULT_RESOURCES = 'build/default_resources'
writeResourceFile('build/default')
FileUtils.mv('build/resources', DEFAULT_RESOURCES)
FileUtils.mv('build/MAHeaders.h', 'build/default_MAHeaders.h')

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
	cName = "build/argv-#{baseName}.c"
	sName = "build/argv-#{baseName}.s"
	return sName if(!force_rebuild)
	writeArgvFile(cName, argv, testSrcName)
	sh "#{MOSYNCDIR}/bin/xgcc -g -I#{MOSYNCDIR}/include/newlib -Werror -S #{File.expand_path(cName)} -o #{sName}"
	return sName
end

DEFAULT_ARGV_SFILE = doArgv('default', DEFAULT_ARGV, 'default', true)


def simple_compile(name)
	sh "#{MOSYNCDIR}/bin/xgcc -g -Werror -S #{File.expand_path(name+'.c')} -o build/#{name}.s#{GCC_FLAGS}"
end

simple_compile('helpers')
simple_compile('setup_filesystem')

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
p dirs
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

def dos2unixInPlaceCommand
	v = open('|dos2unix --version 2>&1').read.strip
	if(v.include?('version 0.'))
		# in-place conversion is default; must specify source and target formats.
		return 'dos2unix --d2u'
	else
		# source and target formats are default; must specify in-place conversion.
		return 'dos2unix -o'
	end
end

LOADER_URLS_FILE = open(SETTINGS[:htdocs_dir] + 'libc_tests.urls', 'wb') if(SETTINGS[:htdocs_dir])

USE_SLD = !SETTINGS[:test_release] && SETTINGS[:use_sld]

def link_and_test(ofn, argvs, files, dead_code, force_rebuild, inputs, code)
	suffix = dead_code ? 'e' : ''
	pfn = ofn.ext('.moo' + suffix)
	winFile = ofn.ext('.win' + suffix)
	failFile = ofn.ext('.fail' + suffix)
	logFile = ofn.ext('.log' + suffix)
	mdsFile = ofn.ext('.md.s')
	esFile = ofn.ext('.e.s')
	sldFile = ofn.ext('.sld' + suffix) if(USE_SLD)
	stabsFile = ofn.ext('.stabs' + suffix) if(USE_SLD)
	
	delete_if_empty(pfn)
	
	# link
	if(!File.exists?(pfn) || force_rebuild)
		pipetool = "#{MOSYNCDIR}/bin/pipe-tool"
		mdFlag = ' -master-dump' if(SETTINGS[:write_master_dump])
		sldFlag = " -sld=#{sldFile}" if(USE_SLD)
		stabsFlags = " -stabs=#{stabsFile}" if(USE_SLD)
		if(dead_code)
			sh "#{pipetool}#{PIPE_FLAGS} -elim#{mdFlag} -B #{pfn} #{ofn} #{argvs} #{PIPE_LIBS}"
			sh "#{pipetool}#{PIPE_FLAGS}#{sldFlag} -B #{pfn} rebuild.s"
		else
			sh "#{pipetool}#{mdFlag}#{sldFlag}#{stabsFlags}#{PIPE_FLAGS} -B #{pfn} #{ofn} #{argvs} #{PIPE_LIBS}"
		end
		force_rebuild = true
	end
	delete_if_empty(pfn)
	if(!File.exists?(pfn))
		error"Unknown link failure."
	end
	
	# execute it, if not win already, or we rebuilt something.
	
	shouldSkipTest = (File.exists?(winFile) || !SETTINGS[:retry_failed]) && !force_rebuild
	return force_rebuild if(shouldSkipTest && !SETTINGS[:force_copy_htdocs])
	
	clear_filesystem
	has_files = false
	
	# copy files only when executing
	files.each do |file|
		has_files = true
		FileUtils.cp_r(file, 'filesystem/')
	end
	if(HOST == :win32)
		cmd = dos2unixInPlaceCommand
		inputs.each do |input|
			sh "#{cmd} \"filesystem/#{File.basename(input)}\""
		end
	end
	
	if(code)
		has_files = true
		code.call
	end

	if(has_files)
		resFile = ofn.ext('.res')
		writeResourceFile(ofn)
		FileUtils.mv('build/resources', resFile)
		#FileUtils.mv('build/MAHeaders.h', 'build/'+ofn+'_MAHeaders.h')
	else
		resFile = DEFAULT_RESOURCES
	end

	# setup for loader
	doCopyToHtdocs = ((!!dead_code == !!SETTINGS[:copy_dce]) && SETTINGS[:htdocs_dir])
	if(doCopyToHtdocs)
		puts 'Copying to htdocs...'
		bn = File.basename(pfn)
		lfn = SETTINGS[:htdocs_dir] + bn
		
		# copy program file
		FileUtils.cp(pfn, lfn)
		# append resource file
		open(lfn, 'ab') do |f|
			f.write(open(resFile, 'rb').read)
		end
		
		LOADER_URLS_FILE.puts(SETTINGS[:loader_base_url] + bn)
		LOADER_URLS_FILE.flush
	end
	
	return force_rebuild if(shouldSkipTest)
	
	sldFlag = " -sld #{sldFile}" if(USE_SLD)
	cmd = "#{MOSYNCDIR}/bin/MoRE -timeout 600 -allowdivzero -noscreen -program #{pfn}#{sldFlag} -resource #{resFile}"
	$stderr.puts cmd
	startTime = Time.now
	if(HOST == :win32)
		cmd = "echo_error.bat #{cmd}"
		res = open('|'+cmd).read.strip
		p res
		res = (res == '0')
	else
		res = system(cmd)
		$stderr.puts $?
	end
	endTime = Time.now
	puts res
	puts "Elapsed time: #{endTime - startTime}"
	if(res == true)	# success
		FileUtils.touch(winFile)
		FileUtils.rm_f(failFile)
		if(!SETTINGS[:keep_parts])
			FileUtils.rm_f(logFile)
			FileUtils.rm_f(mdsFile)
			FileUtils.rm_f(esFile)
			FileUtils.rm_f(sldFile) if(USE_SLD)
			FileUtils.rm_f(stabsFile) if(USE_SLD)
		end
	else	# failure
		FileUtils.touch(failFile)
		FileUtils.rm_f(winFile)
		FileUtils.mv('log.txt', logFile) if(File.exists?('log.txt'))
		FileUtils.mv('_masterdump.s', mdsFile) if(File.exists?('_masterdump.s') && SETTINGS[:write_master_dump])
		FileUtils.mv('rebuild.s', esFile) if(File.exists?('rebuild.s'))
		if(SETTINGS[:stop_on_fail])
			if(SETTINGS[:copy_targets])
				SETTINGS[:copy_targets].each do |target|
					# copy program, sld and stabs to directory :copy_target.
					FileUtils.cp(pfn, target + 'program')
					copyOrRemove(sldFile, target + 'sld.tab', !USE_SLD)
					copyOrRemove(stabsFile, target + 'stabs.tab', dead_code || !USE_SLD)
				end
			end
			error "Stop on fail"
		end
	end
	return force_rebuild
end

def copyOrRemove(src, dst, remove)
	if(remove)
		FileUtils.rm_f(dst)
	else
		FileUtils.cp(src, dst)
	end
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
wins = 0
dceWins = 0

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
	
	unless(SETTINGS[:dce_only])
		force_rebuild = link_and_test(ofn, argvs, files, false, force_rebuild, inputs, code)
		wins += 1 if(File.exists?(ofn.ext('.win')))
	end
	if(SETTINGS[:test_dead_code_elimination] && File.exists?(ofn.ext('.win')))
		link_and_test(ofn, argvs, files, true, force_rebuild, inputs, code)
		dceWins += 1 if(File.exists?(ofn.ext('.wine')))
	end
end

puts "#{unskippedCount} actual tests."
puts "#{SKIPPED_UNRESOLVED.size} known unresolved fails."
puts "#{wins} wins. #{unskippedCount - wins} remains." unless(SETTINGS[:dce_only])
if(SETTINGS[:test_dead_code_elimination])
	puts "#{dceWins} DCE wins. #{unskippedCount - dceWins} remains."
end
