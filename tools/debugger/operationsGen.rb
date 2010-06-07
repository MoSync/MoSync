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

require "../../intlibs/stabs/types.rb"

$binary_operations_multiplicative = [
	["TOKEN_SLASH", "/"],
	["TOKEN_STAR", "*"]
]

$binary_operations = [
	["TOKEN_PLUS", "+"],
	["TOKEN_MINUS", "-"]
	] + $binary_operations_multiplicative

$binary_operations_int = [
	["TOKEN_PERCENT", "%"],
	["TOKEN_SHL", "<<"],
	["TOKEN_SHR", ">>"],
	["TOKEN_AND", "&"],
	["TOKEN_OR", "|"],
	["TOKEN_XOR", "^"]
]

$binary_operations_boolean = [
	["TOKEN_ANDAND", "&&"],
	["TOKEN_OROR", "||"],
	["TOKEN_EQ", "=="],
	["TOKEN_NEQ", "!="],
	["TOKEN_LEQ", "<="],
	["TOKEN_GEQ", ">="],
	["TOKEN_LE", "<"],
	["TOKEN_GE", ">"]
]

$unary_operations = [
	["TOKEN_PLUS", "+"],
	["TOKEN_MINUS", "-"]
]

$unary_operations_int = [
	["TOKEN_TILDE", "~"]
]
	
$unary_operations_boolean = [
	["TOKEN_NOT", "!"],
]

require 'set'

def generate_operations_header(dest)
	out = File.new(dest, "w")
	
	out.write "typedef Value (*BinaryOpFunc)(const Value& a, const Value& b);\n"
	out.write "typedef Value (*UnaryOpFunc)(const Value& a);\n"
	
	#out.write "std::map<int, std::map<int, std::map<int, BinaryOpFunc> > > binaryOpMap;\n"
	#out.write "std::map<int, std::map<int, UnaryOpFunc> > unaryOpMap;\n"
	out.write "static UnaryOpFunc unaryOpMap[_TOKEN_END][Builtin::NUM_SUB_TYPES];\n"
	out.write "static BinaryOpFunc binaryOpMap[_TOKEN_END][Builtin::NUM_SUB_TYPES][Builtin::NUM_SUB_TYPES];\n"
	
	out.write "static void initMaps();\n"
	
	types = $types_int + $types_float + $types_bool
	
	# generate unary operations	
	operations = $binary_operations + $binary_operations_int + $binary_operations_boolean
	

	operations.each do |i|
		out.write "Value operator"+i[1]+"(const Value& v) {\n"
		out.write "\treturn binaryOpMap["+i[0]+"][mPrimitiveType][v.getPrimitiveType()](*this, v);\n"
		out.write "}\n"	
	end
	
	out.write "Value doBinaryOperation(const Value& v, const Token& t) {\n"
	out.write "\treturn binaryOpMap[t.getTokenType()][mPrimitiveType][v.getPrimitiveType()](*this, v);\n"
	out.write "}\n"
	
	# generate unary operations
	operations = $unary_operations + $unary_operations_int + $unary_operations_boolean
		
	operations.each do |i|
		out.write "Value operator"+i[1]+"() {\n"
		out.write "\treturn unaryOpMap["+i[0]+"][mPrimitiveType](*this);\n"
		out.write "}\n"
	end
	
	out.write "Value doUnaryOperation(const Token& t) {\n"
	out.write "\treturn unaryOpMap[t.getTokenType()][mPrimitiveType](*this);\n"
	out.write "}\n"	
	
	out.close()
end

def generate_operations_cpp(dest)
	out = File.new(dest, "w")

	types = $types_int + $types_float + $types_bool
	
	# generate unary operations	
	binaryOperations = $binary_operations + $binary_operations_int +	$binary_operations_boolean
	
	binaryOperations.each do |i|
		out.write "template<typename T1, typename T2>\n"
		out.write "Value do_" + i[0] + "(const Value& a, const Value& b) {\n"
		out.write "\treturn Value((T1)a" + i[1] + "(T2)b);\n";
		out.write "}\n"	
	end	
	
	# generate template specializations for invalid operations
	invalidTypes = ["double", "float"];
	invalidSet = Set.new [["double", "double"]]
	invalidOperations = $binary_operations_int
	invalidTypes.each do |i|
		types.each do |j|
			invalidSet.add [i, j[0]]
			invalidSet.add [j[0], i]					
			invalidSet.add [i, i]		
		end
	end
	
	invalidOperations.each do |i|
		invalidSet.each do |j| 
		out.write "template<>\n"
		out.write "Value do_"+i[0]+"<"+ j[0] + ", " + j[1] + ">(const Value& a, const Value& b) {\n"
		out.write "\tthrow ParseException(\"Invalid operation " + i[1] + " for types " + j[0] + " and " + j[1] +".\");\n";
		out.write "}\n"
		end
	end	
	
	# generate unary operations
	unaryOperations = $unary_operations + $unary_operations_int + $unary_operations_boolean
	
		
	unaryOperations.each do |i|
		out.write "template<typename T>\n"
		out.write "Value do_"+i[0]+"(const Value& a) {\n"	
		out.write "\treturn Value(" + i[1] + "(T)a);\n";
		out.write "}\n"
	end	
	
	invalidTypes = ["double", "float"]
	invalidOperations = $unary_operations_int
	
	invalidOperations.each do |i|
		invalidTypes.each do |j| 
			out.write "template<>\n"
			out.write "Value do_"+i[0]+"<"+j+">(const Value& a) {\n"
			out.write "\tthrow ParseException(\"Invalid operation " + i[1] + " for type " + j + ".\");\n";
			out.write "}\n"	
		end
	end	
	
	# generate map for stuff
	out.write "Value::UnaryOpFunc Value::unaryOpMap[_TOKEN_END][Builtin::NUM_SUB_TYPES];\n"
	out.write "Value::BinaryOpFunc Value::binaryOpMap[_TOKEN_END][Builtin::NUM_SUB_TYPES][Builtin::NUM_SUB_TYPES];\n"

	out.write "void Value::initMaps() {\n"
	out.write "\tstatic bool mapsInitialized = false;\n"
	out.write "\tif(mapsInitialized) return;\n"
	out.write "\tmapsInitialized = true;\n"
	binaryOperations.each do |i|
		types.each do |j|
			types.each do |k| 
				out.write "\tbinaryOpMap["+i[0]+"][Builtin::e" + j[1] + "][Builtin::e" + k[1] + "] = do_"+i[0]+"<"+j[0]+","+k[0]+">;\n";
			end
		end
	end
	
	unaryOperations.each do |i|
		types.each do |j|
			out.write "\tunaryOpMap["+i[0]+"][Builtin::e" + j[1] + "] = do_"+i[0]+"<"+j[0]+">;\n";
		end
	end	
	out.write "}\n"
	

	out.close()
end

generate_operations_cpp("operations_generated.cpp")
generate_operations_header("operations_generated.h")
