# Copyright (C) 2009 Mobile Sorcery AB
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

require "./types.rb"

def generate_builtins_macro(dest)
	out = File.new(dest, "w")
	types = $types_int + $types_float # + $types_bool
	out.write "#define BUILTINS(f, m)\\\n"
	j = 0
	types.each do |i|
		if(j==0)
			out.write "f(" + i[0] + "," + i[1] + ")\\\n"
		else
			out.write "m(" + i[0] + "," + i[1] + ")\\\n"		
		end
		
		j=j+1
	end
	out.write "\n"

	out.write "#define BROKEN_BUILTINS(m)\\\n"
	$types_broken.each do |i|
		out.write "m(" + i[0] + "," + i[1] + ")\\\n"
	end
	out.write "\n"
	
	out.close()	
end

generate_builtins_macro("stabs_builtins_generated.h")
