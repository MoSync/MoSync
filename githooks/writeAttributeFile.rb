#!/usr/bin/ruby

# Create a .gitattributes file based on other_licenses.txt,
# to exclude its files from git diff-index --check.

BASEDIR = File.expand_path("#{File.dirname(__FILE__)}/..")

outName = "#{BASEDIR}/.gitattributes"
inName = "#{BASEDIR}/other_licenses.txt"

CHECK_PATTERNS = [
	'workfile.rb',
	'/templates',
	'/githooks',
]

class String
	def includeOneOf?(array)
		array.each do |s|
			return true if(self.include?(s))
		end
		return false
	end
end

raise '.gitattributes exists!' if(File.exist?(outName))
@outFile = open(outName, 'w')
inFile = open(inName, 'r')
def putLine(line)
	@outFile.puts line.gsub(' ', '.') + ' -diff' unless(line.includeOneOf?(CHECK_PATTERNS))
end
inFile.each do |line|
	line.strip!
	next if(line.length == 0)
	test = File.expand_path(File.join(BASEDIR, line))
	if(File.directory?(test))
		dir = File.join(test, '**/*')
		Dir[dir].each do |file|
			putLine(file[BASEDIR.size..-1])
		end
	else
		putLine(line)
	end
end

@outFile.close
inFile.close
