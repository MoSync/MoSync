# check all modified text files for invalid utf-8.

# note: this script does not check the indexed versions of the files;
# but rather the ones on disk.
# I think we can live with that for now, until someone figures out how to
# parse the output from 'git diff'.

# get the list of changed files. parse it to find filenames and binaries.
error = false
open('|git diff-index --numstat --cached HEAD --').each do |line|
	added, deleted, name = line.split(' ', 3)
	if(added == '-')	# binary
		raise hell if(deleted != '-')
		next
	end
	name.strip!
	if(!File.exist?(name))
		if(added != '0')
			puts name
			raise hell
		end
		next
	end
	file = open(name, 'r')
	count = 0
	file.each do |fl|
		count += 1
		next if(fl.force_encoding("UTF-8").valid_encoding?)

		puts "Invalid UTF-8 sequence detected:" if(!error)
		puts "#{name}:#{count}"
		error = true
	end
end

raise "Invalid UTF-8 sequence detected" if(error)
