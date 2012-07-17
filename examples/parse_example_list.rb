#!/usr/bin/ruby

#
# parses the examples list uses by eclipse to provide unique processing of examples
#
def parseExampleList
	subDirs = Array.new
	type = ""
	File.open("examples.list").each do |line|
		if((!line.include?("#"))&&(line.strip.start_with?("["))&&(line.strip.end_with?("]")))
			type = line.strip[1..-2]
		end
		if((!line.include?("#"))&&(!line.include?("[")))
			if(line.include?("="))
				lineSplit = type + "/" + line.split("=")[1].strip
				if(File.exist?(lineSplit))
					subDirs.push(lineSplit)
				end
			else
				lineStrip = type + "/" + line.strip
				if(File.exist?(lineStrip))
					subDirs.push(lineStrip)
				end
			end
		end
	end
	return subDirs
end
