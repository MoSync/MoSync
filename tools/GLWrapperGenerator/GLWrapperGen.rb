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

class GLWrapperGen

	def initialize()
		@@gl_to_mosync_types =
		{
			'GLenum' 	=> 'int',
			'GLuint' 	=> 'int',
			'GLint' 	=> 'int',
			'GLfloat' 	=> 'float',
			'GLclampf' 	=> 'float',
			'GLbitfield' 	=> 'int',
			'GLboolean' 	=> 'int',	
			'GLfixed' 	=> 'int',
			'GLvoid' 	=> 'void',
			'void' 		=> 'void',
			'GLsizei' 	=> 'int',
			'GLclampx' 	=> 'int',
			'GLsizeiptr' 	=> 'int',
			'GLintptr' 	=> 'int',
			'GLubyte' 	=> 'int',
		}
	
		@@invoke_exceptions = ["glGetString"]
	
		@@Argument = Struct.new(:is_const, :data_type, :mosync_type, :is_pointer, :argument_name, :call_argument_name, :array_size) 
		
	end
	
	
	
	def build_idl_wrapper_arguments(argument_list)
		new_arguments = ""
		
		argument_list.each do |argument|
			is_const = argument.is_const
			data_type = argument.data_type
			is_pointer = argument.is_pointer			
			argument_name = argument.argument_name
			is_array = argument.array_size
				
#			puts 'data_type: ' + data_type
#			puts 'is_pointer: ' + is_pointer
#			puts 'argument_name: ' + argument_name	
#			puts 'is_array: ' + is_array	
#			puts 'is_const: ' + is_const		
				
			if(argument_list.length < 4)
				if(is_pointer != '')
					if(is_const != "")
						new_arguments += 'in '
					else
						new_arguments += 'out '
					end
				else
					new_arguments += 'in '
				end
			end
					
			
				
			if(is_pointer == '**' || is_pointer == '*')
			
#				puts "is_const: \'" + is_const + "\'"
				new_arguments += 'MAAddress ' + argument_name;
			else
				if(@@gl_to_mosync_types[data_type] == nil)
					puts "error: " + data_type
					return nil
				end

				new_arguments += @@gl_to_mosync_types[data_type] + ' ' + argument_name;
			end
			
			if(argument_list.length > 3)
				new_arguments += '; '
			else
				if(argument != argument_list.last())
					new_arguments += ', '
				end	
			end
		end
		
		return new_arguments
	end
	
	
	def build_java_wrapper_arguments(argument_list)
		new_call_arguments = ""
		pre_code = ""
		post_code = ""
		
		argument_list.each do |argument|
			is_const = argument.is_const
			data_type = argument.data_type
			is_pointer = argument.is_pointer			
			argument_name = argument.call_argument_name
			is_array = argument.array_size
				
#			puts 'data_type: ' + data_type
#			puts 'is_pointer: ' + is_pointer
#			puts 'argument_name: ' + argument_name	
#			puts 'is_array: ' + is_array	
#			puts 'is_const: ' + is_const		
				
			if(is_pointer == '**')
				buffer_name = argument_name + "_buffer";
				pre_code+="\tBuffer " + buffer_name + "[] = new Buffer[1];\n"
				new_call_arguments += buffer_name
				post_code+="\tmCore.mMem_ds[" + argument_name + "] = getResourceHandle(" + buffer_name + "[0]);\n"
			elsif(is_pointer == '*')
				
				if(data_type == "GLfixed" || data_type == "GLint" || data_type == "GLuint" || data_type == "GLboolean")
					new_call_arguments += "((ByteBuffer)getResource(" + argument_name + ")).asIntBuffer()"
				elsif(data_type == "GLfloat")
					new_call_arguments += "((ByteBuffer)getResource(" + argument_name + ")).asFloatBuffer()"
				elsif(data_type == "GLshort" || data_type == "GLushort")
					new_call_arguments += "((ByteBuffer)getResource(" + argument_name + ")).asShortBuffer()"
				elsif(data_type == "GLbyte" || data_type == "GLubyte")
					new_call_arguments += "((ByteBuffer)getResource(" + argument_name + "))"					
				elsif(data_type == "GLvoid")
					new_call_arguments += "((Buffer)getResource(" + argument_name + "))"
				end				
							
				#if(is_array != "")
					#new_call_arguments += "IntBuffer.wrap(mCore.mMem_ds, " + argument_name + ", " + is_array + ")"
				#	new_call_arguments += "(ByteBuffer) getResource(" + argument_name + ")"
				#else
					#new_call_arguments += "IntBuffer.wrap(mCore.mMem_ds, " + argument_name + ", mCore.mMem_ds.length - " + argument_name + ")"
				#	new_call_arguments += "(ByteBuffer) getResource(" + argument_name + ")"
				#end
			else
				if(@@gl_to_mosync_types[data_type] == nil)
					puts "error: " + data_type
					return nil
				end

				if(data_type  == 'GLubyte') 
					new_call_arguments += '(byte)'	
				elsif(@@gl_to_mosync_types[data_type]  == 'float') 
					new_call_arguments += 'Float.intBitsToFloat('
				end


				if(data_type == '') 
					new_call_arguments += '(byte)'
				end	

				new_call_arguments += argument_name	

				if(data_type == 'GLboolean') 
					new_call_arguments += '!=0'
				elsif(@@gl_to_mosync_types[data_type]  == 'float') 
					new_call_arguments += ')'
				end
				
			end
			
			if(argument != argument_list.last())
				new_call_arguments += ', '
			end			
		end
		
		return [0, pre_code, post_code, new_call_arguments]
	end	

	def read_string(s)
		if(s == nil) 
			return ""
		else
			return s.strip
		end
	end
	

	def parse_argument_list(argument_list)			
		parsed_arguments = Array.new
		
		argument_list = argument_list.split(",")
		
		letters = ['a', 'b', 'c']
		letterIndex = 0
		
		argument_list.each do |argument|
			argument.strip!
			
##		GL_API void GL_APIENTRY glLightxv (GLenum light, GLenum pname, const GLfixed *params);
			match = /((const\s+)?)\s*(\w+)\s*(\*{0,2})\s*(\w*)\s*(\[(.*)\])?/.match(argument)
			
			if(match)
				is_const = read_string(match[2])
				data_type = read_string(match[3])
				is_pointer = read_string(match[4])			
				argument_name = read_string(match[5])
				is_array = read_string(match[7])
				
				if(is_array != "")
					puts "static array! "  + argument
					is_pointer = "*"
					is_array = is_array.strip
				end
				
				if(argument_name != "")	
					if( argument_list.length > 3)
						parsed_arguments.push(@@Argument.new(is_const, data_type, @@gl_to_mosync_types[data_type], is_pointer, argument_name, argument_name, is_array))
					else
						parsed_arguments.push(@@Argument.new(is_const, data_type, @@gl_to_mosync_types[data_type], is_pointer, argument_name, letters[letterIndex], is_array))						
					end
					
#					puts 'data_type: ' + data_type
#					puts 'is_pointer: ' + is_pointer
#					puts 'argument_name: ' + argument_name	
#					puts 'is_array: ' + is_array	
#					puts 'is_const: ' + is_const	
				end
				
				letterIndex += 1
			end				
		end
		
		return parsed_arguments
	end

	def parse_return_type(return_type)
		match = /((const)?\s*)(.+)/.match(return_type)
		#puts "return_type: \'" + return_type + "\'"

		is_const = match[1].strip
		
		## fugly hack to check for pointer, because i had a bad regexp hair day..
		## only works with single pointers (no double pointers in ogles as return types)
		splitted = match[3].strip.split(" ");
		type = splitted[0]
		if(splitted[1] != nil)
			is_pointer = "*"
		else
			is_pointer = ""
		end
		
		#puts "is_const: " + is_const
		#puts "type: " + type
		#puts "is_pointer: " + is_pointer
						
		return [is_const, is_pointer, type]
	end
	
	def generate_idl_info(function_name, return_type, contains_pointer, args)
		return_string = ""
		
		if(contains_pointer==true)
			contains_pointer = "Buffer"
		else
			contains_pointer = ""
		end
			
		if(args.length>3)
			struct_name = function_name + 'Struct';	
			return_string+='struct ' + struct_name + ' { ' + build_idl_wrapper_arguments(args) + " }\n"		
			return_string+='int ' + function_name + contains_pointer + 'Temp( in ' + struct_name + " struct_ptr);\n"	
		else
			return_string+='int ' + function_name + contains_pointer + 'Temp(' + build_idl_wrapper_arguments(args) + ");\n"
		end
			
		return return_string
	end
	
	def contains_pointer?(args, return_type)
		has_p = (return_type[1] != '')
		args.each do |argument| 
			if(argument.is_pointer != '')
				has_p = true;	
			end
		end	
		return has_p
	end
	
	def generate_java_invoke(java_call, function_name, return_type, contains_pointer, args)
		return_string = ""
				
		if(contains_pointer == true)
			return_string += "case maIOCtl_" + function_name + "BufferTemp: {\n"
		else
			return_string += "case maIOCtl_" + function_name + "Temp: {\n"
		end
		
		if(args.length>3)
			
			offset = 0
			args.each do |argument| 
				# if more data types are needed add proper retrieving of them here (only float and int exist in common profile)
#				if(@@gl_to_mosync_types[argument.data_type] == "float")
#					return_string += "\tfloat " + argument.argument_name + " = Float.intBitsToFloat(RINT(a + " + offset.to_s + "));\n"
#				else
					return_string += "\tint " + argument.argument_name + " = RINT(a + " + offset.to_s + ");\n"					
#				end
				
				offset += 4
			end
				
		end;

		return_string += java_call

		return_string+="}\n"

		return return_string
	end
	
	def get_opengl_return_type_cast_string(return_type)
		return_string = ""

		return_string += "("
						
		if(return_type[0] != "")
			return_string += return_type[0] + " "
		end

		
		if(return_type[2] != "")
			return_string += return_type[2] + " "
		end	
			
			
		if(return_type[1] != "")
			return_string += return_type[1] + " "
		end	
						
		return_string += ")"	
	end	
	
	def get_mosync_type_cast_string(argument)
		return_string = ""

		return_string += "("
						
		if(argument.is_const != "")
			return_string += argument.is_const + " "
		end

			
		if(argument.is_pointer != "")
			return_string += "void *"
		else
			return_string += argument.mosync_type + " "		
		end			
						
		return_string += ")"	
	end
	
	def generate_mosync_header(line, function_name, return_type, contains_pointer, args)
		return_string = ""
		
		line = line.strip
		line = line[0..line.length-2]
		
		
		if(contains_pointer==true)
			contains_pointer = "Buffer"
		else
			contains_pointer = ""
		end	
		
		return_string += "inline " + line + " {\n"
		
		if(args.length > 3)
			return_string += "\tconst " + function_name + "Struct param_struct = {"
			args.each do |argument| 
				if(argument.mosync_type != "void")
					return_string += get_mosync_type_cast_string(argument)
				else
					if(argument.is_pointer != "")
						return_string += get_mosync_type_cast_string(argument)	
					end
					
				end
				
				return_string += argument.argument_name
				if(argument != args.last)
					return_string += ","
				end
			end	
			return_string += "};\n"
			
			return_string += "\t"
			if(return_type[2] != "void" || (return_type[2] == "void" && return_type[1] != ""))
				return_string += "return " + get_opengl_return_type_cast_string(return_type) + " ";
			end
			
			return_string += function_name + contains_pointer + "Temp(&param_struct);\n"
		
		else
			
			return_string += "\t"
			
			if(return_type[2] != "void" || (return_type[2] == "void" && return_type[1] != ""))
				return_string += "return " + get_opengl_return_type_cast_string(return_type) + " ";
			end
			
			return_string += function_name + contains_pointer + "Temp("
			args.each do |argument| 
				if(argument.mosync_type != "void")
					return_string += get_mosync_type_cast_string(argument)
				else
					if(argument.is_pointer != "")
						return_string += get_mosync_type_cast_string(argument)	
					end	
				end
				
				return_string += argument.argument_name
				if(argument != args.last)
					return_string += ","
				end
			end	
			return_string += ");\n"
		end
		
		return_string += "}\n"
		
		return return_string
	end
	
	def generate_java_call(function_name, return_type, java_info)
		finalFunc = ""
		#finalFunc =
		#@@gl_to_mosync_types[return_type] + " " + function_name + "(" + java_info[0] + ") {\n"
		if(java_info[1]!="")
			finalFunc += java_info[1]
		end

		finalFunc += "\t"

		if(@@gl_to_mosync_types[return_type] != 'void') 
			finalFunc += "return "
		end
			
		finalFunc += "gl." + function_name + "(" + java_info[3] + ")"
		
		if(return_type == "GLboolean")
			finalFunc += "==true?1:0"			
		end
		
		finalFunc += ";\n"
		
		if(java_info[2]!="") 
			finalFunc += java_info[2]
		end
		
		if(@@gl_to_mosync_types[return_type] == 'void') 
					finalFunc += "\treturn 0;\n"
		end
		

		#finalFunc += "}\n\n"
		return finalFunc
	end	
	
	def generate_wrappers(idl_file, java_invoke_file, mosync_header_file, line)
		match = /GL_API (.+) GL_APIENTRY (.+)\((.*)\);/.match(line)
		
		if (match)
			return_type = match[1].strip
			function_name = match[2].strip
			argument_list = match[3].strip

#			puts 'return_type: ' + return_type
#			puts 'function_name: ' + function_name
#			puts 'argument_list: ' + argument_list
			
			args = parse_argument_list(argument_list)
			
			if(args != nil)
				java_info = build_java_wrapper_arguments(args)
				return_type = parse_return_type(return_type)
			
				contains_pointer = contains_pointer?(args, return_type)
			
				if(
					#return_type[0] == "" && return_type[1] == "" && 
					java_info != nil)		
					idl_file.puts generate_idl_info(function_name, return_type[2], contains_pointer, args)						
					java_call = generate_java_call(function_name, return_type[2], java_info)
					
					if(@@invoke_exceptions.include?(function_name) != true)
						java_invoke_file.puts generate_java_invoke(java_call, function_name, return_type[2], contains_pointer, args)	
					end
					
					mosync_header_file.puts generate_mosync_header(line, function_name, return_type, contains_pointer, args)			
				end
			end
		end
	end


	def generate(filename)	
		begin
			File.open(filename, 'r') do |file|
			File.open("opengl_generated.idl", 'w') do |idl_file|
			File.open("opengl_invoke_generated.jpp", 'w') do |java_invoke_file|
			File.open("magl_functions.h", 'w') do |mosync_header_file|
			while ( line = file.gets )
				generate_wrappers(idl_file, java_invoke_file, mosync_header_file, line)
			end
			end
			end
			end
			end
		rescue => err
			puts "Exception: #{err}\n#{err.backtrace.join("\n")}"
			err
		end
	end
end

GLWrapperGen.new.generate("gl_common_lite.h")
