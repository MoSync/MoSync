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

$types_int = [
	["int", "Int"],
	["char", "Char", "data"],
	["long int", "LongInt", "data"],
	["unsigned int", "UnsignedInt", "data"],
	["long unsigned int", "LongUnsignedInt"],
	["long long int", "LongLongInt"],
	["long long unsigned int", "LongLongUnsignedInt"],
	["short int", "ShortInt"],
	["short unsigned int", "ShortUnsignedInt"],
	["signed char", "SignedChar"],
	["unsigned char", "UnsignedChar"],
	["wchar_t", "Wchar"]
]

$types_float = [
	["float", "Float"],
	["double", "Double"]
]

$types_bool = [
	["bool", "Bool"]
]

$types_broken = [
	["long double", "LongDouble"],
	["complex int", "ComplexInt"],
	["complex float", "ComplexFloat"],
	["complex double", "ComplexDouble"],
	["complex long double", "ComplexLongDouble"],
	["void", "Void"],
	["nothrow_t", "Nothrow"]
]
