def error(msg)
	puts "Error: #{msg}"
	raise msg
end

def warning(msg)
	puts "#{msg}"
end

def aprint(a)
	print "["
	first = true
	a.each do |item|
		if(first) then
			first = false
		else
			print ", "
		end
		print item
	end
	print "]\n"
end
