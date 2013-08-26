# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
