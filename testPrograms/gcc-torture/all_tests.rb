#!/usr/bin/ruby

require 'FileUtils'
require 'settings.rb'
require 'skipped.rb'
require '../../rules/util.rb'

BUILD_DIR = 'build'
MOSYNCDIR = ENV['MOSYNCDIR']
GCC_FLAGS = " -I. -I#{MOSYNCDIR}/include -DNO_TRAMPOLINES"
PIPE_FLAGS = " #{MOSYNCDIR}/lib/pipe_debug/mastd.lib"

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
puts files.count
files.each do |filename|
	bn = File.basename(filename)
	if(SKIPPED.include?(bn))
		puts "Skipped #{bn}"
		next
	end
	puts bn
	ofn = BUILD_DIR + '/' + bn.ext('.s')
	pfn = ofn.ext('.moo')
	sh "#{MOSYNCDIR}/bin/xgcc -g -S \"#{filename}\" -o #{ofn}#{GCC_FLAGS}" unless(File.exists?(ofn))
	sh "pipe-tool -B #{pfn} #{ofn} build/helpers.s#{PIPE_FLAGS}" unless(File.exists?(pfn))
end
