/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * TestBitset.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
 */

/**
 * @file TestBitset.cpp
 * @author Gabriela Rata
 */


#include <bitset>
#include <string>
#include <assert.h>
#include "../STLMoblet.h"
#include "../LOG.h"

void STLMoblet::STL_bitset()
{
	using std::bitset;

	LOG("\n");
	LOG("========================= bitset ================================================================================");

	LOG("/**");
	LOG("*  bitset is a special container class that is designed to store bits.");
	LOG("*  It is optimized for space allocation: each element occupies only one bit");
	LOG("*  bitset is defined as a class template taking one parameter (of type unsigned int):");
	LOG("*  			template < size_t N > class bitset;");
	LOG("*  			N = the number of bits to represent");
	LOG("*  It is defined in the <bitset> header.");
	LOG("*/");

	LOG("\n");
	LOG("/**");
	LOG("*  default constructor");
	LOG("*/");

	TRACE(bitset<8> b1;);

	LOG("\n");
	LOG("/**");
	LOG("*  constructor");
	LOG("*/");

	TRACE(std::string bits("00001111"););
	bitset<8> b2(bits);
	log_to_console<8>(b2, "bitset<8> b2(bits);	//b2 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG("*  operator[]: The function returns the reference to a bit.");
	LOG("*/");

	int bit = b2[0];
	log_to_console(int(bit), "int bit = b2[0];	//bit = ");

	bit = b2[1];
	log_to_console(int(bit), "bit = b2[1];	//bit = ");

	bit = b2[2];
	log_to_console(int(bit), "bit = b2[2];	//bit = ");

	bit = b2[3];
	log_to_console(int(bit), "bit = b2[3];	//bit = ");

	bit = b2[4];
	log_to_console(int(bit), "bit = b2[4];	//bit = ");

	bit = b2[5];
	log_to_console(int(bit), "bit = b2[5];	//bit = ");

	bit = b2[6];
	log_to_console(int(bit), "bit = b2[6];	//bit = ");

	bit = b2[7];
	log_to_console(int(bit), "bit = b2[7];	//bit = ");

	LOG("\n");
	LOG("/**");
	LOG("*  set function has two overloads.");
	LOG("*  The version without parameters sets all the bits in the bitset (to 1).");
	LOG("*  The second version takes two parameters: an index and a bool. I will set the bit");
	LOG("*  from that index to 1, for true and 0 for false.");
	LOG("*/");

	b2.set();

	log_to_console<8>(b2, "b2.set();	//b2 will contain now: ");

	b2.set(4, false);
	log_to_console(int(b2[4]), "b2.set(4, false);	//b2[4] = ");

	LOG("\n");
	LOG("/**");
	LOG("*  reset function has two overloads.");
	LOG("*  The version without parameters resets all the bits in the bitset (to 0).");
	LOG("*  The second version takes one parameter: an index. I will reset the bit");
	LOG("*  from that index.");
	LOG("*/");

	b2.reset();
	log_to_console<8>(b2, "b2.reset();	//b2 will contain: ");


	LOG("\n");
	TRACE(b2[3] = 1;);
	b2.reset(3);
	log_to_console((int)b2[3], "b2.reset(3); 	//b2[3] = ");

	LOG("\n");
	LOG("/**");
	LOG("*  flip function has two overloads.");
	LOG("*  The version without parameters flips all the bits in the bitset (from 0 to 1 and from 1 to 0).");
	LOG("*  The second version takes one parameter: an index. I will flip the bit from that index.");
	LOG("*/");

	b2.flip();
	log_to_console<8>(b2, "b2.flip(); 	//b2 will contain: ");

	LOG("\n");
	b2.flip(4);
	log_to_console((int)b2[4], "b2.flip(4); 	//b2[4] = ");

	LOG("\n");
	LOG("/**");
	LOG("* to_ulong function: converts the bitset to a unsigned long");
	LOG("*/");

	b2.reset();
	log_to_console<8>(b2, "b2.reset(); b2 will contain: ");

	LOG("\n");
	unsigned long val = b2.to_ulong();
	log_to_console(val, "unsigned long val = b2.to_ulong(); 	//val = ");

	LOG("\n");
	LOG("/**");
	LOG("* to_string function: converts the bitset to a string");
	LOG("*/");

	std::string strVal = b2.to_string<char, std::char_traits<char>, std::allocator<char> >();
	log_to_console(strVal.c_str(), "std::string strVal = b2.to_string<char, "
			"std::char_traits<char>, std::allocator<char> >(); 	//strVal = ");

	LOG("\n");
	LOG("/**");
	LOG("* any function: returns whether any of the bits in the bitset is 1.");
	LOG("*/");

	bool anyBit = b2.any();
	log_to_console(anyBit, "bool anyBit = b2.any();	//anyBit = ");

	LOG("\n");
	LOG("/**");
	LOG("* none function: return whether none of the bits in the bitset is 1.");
	LOG("*/");

	bool noBitSet = b2.none();
	log_to_console(noBitSet, "bool noBitSet = b2.none();	//noBitSet = ");


	LOG("\n");
	LOG("bitset overloads the following operators:");

	LOG("\n");
	LOG("/**");
	LOG("* operator=");
	LOG("*/");

	TRACE(bitset<8> b2_copy;);
	b2_copy = b2;

	log_to_console<8>(b2_copy, "b2_copy = b2;	//b2_copy = ");

	LOG("\n");
	LOG("/**");
	LOG("* operator==");
	LOG("*/");
	TRACE(assert(b2 == b2_copy););

	LOG("\n");
	LOG("/**");
	LOG("* operator!=");
	LOG("*/");

	TRACE(b2_copy.flip(0););
	TRACE(assert(b2 != b2_copy););

	LOG("\n");
	LOG("/**");
	LOG("* operator^= (bitwise exclusive-or operator)");
	LOG("* a global operator^ is provided also");
	LOG("*/");

	TRACE(bitset<4> lhsBitset(std::string("1001")););
	TRACE(bitset<4> rhsBitset(std::string("0011")););

	LOG("\n");
	lhsBitset ^= rhsBitset;		//1001 XOR 0011 => 1010
	log_to_console<4>(lhsBitset, "lhsBitset ^= rhsBitset;	//1001 XOR 0011 =>"
			" lhsBitset will contain: ");

	LOG("\n");
	LOG("/**");
	LOG("* operator&= (AND operator)");
	LOG("* a global operator & is provided also");
	LOG("*/");

	lhsBitset &= rhsBitset; 						// 1010 & 0011 => 0010
	log_to_console<4>(lhsBitset, "lhsBitset &= rhsBitset;	//1010 & 0011 => lhsBitset will contain: ");

	LOG("\n");
	LOG("/**");
	LOG("* operator|= (OR operator)");
	LOG("* a global operator | is provided also");
	LOG("*/");

	lhsBitset |= rhsBitset; 						// 0010 | 0011 => 0011
	log_to_console<4>(lhsBitset, "lhsBitset |= rhsBitset; 	0010 | 0011 => lshBitset will contain: ");

	LOG("\n");
	LOG("/**");
	LOG("* operator<<= (shift left)");
	LOG("*/");

	lhsBitset<<=1; 						// 0011<<1 => 0110
	log_to_console<4>(lhsBitset, "lhsBitset<<=1; 	//0011<<1 => lhsBitset will contain: ");

	LOG("\n");
	LOG("/**");
	LOG("* operator>>= (shift right)");
	LOG("*/");

	lhsBitset>>=1; 						// 0110>>1 => 0011
	log_to_console<4>(lhsBitset, "lhsBitset>>=1; 	//0110>>1 => lshBitset will contain: ");

	LOG("\n");
	LOG("/**");
	LOG("* operator~ (NOT)");
	LOG("*/");

	log_to_console<4>(rhsBitset, "//rhsBitset contains: ");
	rhsBitset = ~rhsBitset;		//~0011 => 1100
	log_to_console<4>(rhsBitset, "rhsBitset = ~rhsBitset; //~0011 => rhsBitset will be: ");

	LOG("\n");
}
