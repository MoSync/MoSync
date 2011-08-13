#!/usr/bin/ruby

class String
	# Returns true if self begins with with.
	def beginsWith(with)
		return false if(self.length < with.length)
		return self[0, with.length] == with
	end
end

inFunctions = false

File.open("sld.js","w") do |out|
  out << "var functions = {\n"
  File.open("sld.tab").each do |line|
    if(line.beginsWith("FUNCTIONS") == true) 
		inFunctions = true
	elsif(line.beginsWith("VARIABLES")==true) 
		inFunctions = false;
	end
	
	if(inFunctions == true)
		result = /(\w*)\s([0-9a-f]*),([0-9a-f]*)/.match(line)
		if(result != nil)
			out << result[2].to_i(16) << " : \"" << result[1] << "\",\n"
		end
	end
  end
  out << "}\n"
end
