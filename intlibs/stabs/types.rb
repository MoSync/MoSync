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
	["wchar_t", "Wchar"],
]

$types_float = [
	["float", "Float"],
	["double", "Double"],
	["long double", "LongDouble"],
]

$types_bool = [
	["bool", "Bool"],
]

$types_broken = [
	["complex int", "ComplexInt"],
	["complex float", "ComplexFloat"],
	["complex double", "ComplexDouble"],
	["complex long double", "ComplexLongDouble"],
	["void", "Void"],
	["nothrow_t", "Nothrow"],
]
