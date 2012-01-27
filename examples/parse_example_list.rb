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
         lineSplit = line.split("=")[1].strip
         if(File.exist?(lineSplit))
           subDirs.push(type + "/" + lineSplit)
         end
       else
         if(File.exist?(line.strip))
           subDirs.push(type + "/" + line.strip)
         end
       end
   end
 end
 return subDirs
end
