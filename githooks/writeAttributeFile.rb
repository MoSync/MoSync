#!/usr/bin/ruby

# Create a .gitattributes file based on other_licenses.txt,
# to exclude its files from git diff-index --check.

outName = File.expand_path("#{File.dirname(__FILE__)}/../.gitattributes")
inName = File.expand_path("#{File.dirname(__FILE__)}/../other_licenses.txt")

CHECK_PATTERNS = [
	'workfile.rb',
	'/templates',
]

raise '.gitattributes exists!' if(File.exist?(outName))
outFile = open(outName, 'w')
inFile = open(inName, 'r')
inFile.each do |line|
	line.strip!
	next if(line.length == 0)
	outFile.puts line + ' -diff' unless(CHECK_PATTERNS.include?(line))
end
outFile.close
inFile.close
