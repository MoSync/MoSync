require 'find'

def write_array(name, src_filename, out_file)
	in_file = File.new(src_filename, "rb")
	out_file.print "unsigned char #{name}[] = {\n\t"
	i = 0
	in_file.each_byte do |b|
		out_file.print sprintf("0x%x,\t", b)
		i = i + 1
		if(i % 8 == 0) then
			out_file.print "\n\t"
		end
	end
	out_file.puts "\n};\n"
	in_file.close
end

out_file = File.new("../../libs/MAUI-revamp/DefaultSkin.h", "w")

out_file.puts "/** Generated arrays for MAUI default skin **/"

Find.find(".") do |path|
	puts "Found file: #{path}"
	ext = File.extname(path)
	if(ext.eql?(".png") || ext.eql?(".mof")) then
		puts "it's a png!!"
		write_array(File.basename(path.downcase, ext.downcase), path, out_file)
	end
end

out_file.close
