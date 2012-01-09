/*
 * TestBitset.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
 */


#include <bitset>
#include <string>
#include "../TestSTL.h"

/**
*  function for testing bitset
*  bitset is a special container class that is designed to store bits.
*  It is optimized for space allocation: each element occupies only one bit
*  bitset is defined as a class template taking one parameter (of type unsigned int):
*  			template < size_t N > class bitset;
*  			N = the number of bits to represent
*  It is defined in the <bitset> header.
*/
void TestSTL::test_bitset()
{
	using std::bitset;

	/**
	*  default constructor
	*/
	bitset<8> b1;

	/**
	*  constructor
	*/
	std::string bits("00001111");
	bitset<8> b2(bits);

	/**
	*  operator[]: The function returns the reference to a bit  .
	*/
	TESTIFY_ASSERT(b2[0] == 1);
	TESTIFY_ASSERT(b2[1] == 1);
	TESTIFY_ASSERT(b2[2] == 1);
	TESTIFY_ASSERT(b2[3] == 1);
	TESTIFY_ASSERT(b2[4] == 0);
	TESTIFY_ASSERT(b2[5] == 0);
	TESTIFY_ASSERT(b2[6] == 0);
	TESTIFY_ASSERT(b2[7] == 0);

	/**
	*  set function has two overloads.
	*  The version without parameters sets all the bits in the bitset (to 1).
	*  The second version takes two parameters: an index and a bool. I will set the bit
	*  from that index to 1, for true and 0 for false.
	*/
	b2.set();
	TESTIFY_ASSERT(b2[0] == 1);
	TESTIFY_ASSERT(b2[1] == 1);
	TESTIFY_ASSERT(b2[2] == 1);
	TESTIFY_ASSERT(b2[3] == 1);
	TESTIFY_ASSERT(b2[4] == 1);
	TESTIFY_ASSERT(b2[5] == 1);
	TESTIFY_ASSERT(b2[6] == 1);
	TESTIFY_ASSERT(b2[7] == 1);

	b2.set(4, false);
	TESTIFY_ASSERT(b2[4] == 0);

	/**
	*  reset function has two overloads.
	*  The version without parameters resets all the bits in the bitset (to 0).
	*  The second version takes one parameter: an index. I will reset the bit
	*  from that index.
	*/
	b2.reset();
	TESTIFY_ASSERT(b2[0] == 0);
	TESTIFY_ASSERT(b2[1] == 0);
	TESTIFY_ASSERT(b2[2] == 0);
	TESTIFY_ASSERT(b2[3] == 0);
	TESTIFY_ASSERT(b2[4] == 0);
	TESTIFY_ASSERT(b2[5] == 0);
	TESTIFY_ASSERT(b2[6] == 0);
	TESTIFY_ASSERT(b2[7] == 0);

	b2[3] = 1;
	b2.reset(3);
	TESTIFY_ASSERT(b2[3] == 0);

	/**
	*  flip function has two overloads.
	*  The version without parameters flips all the bits in the bitset (from 0 to 1 and from 1 to 0).
	*  The second version takes one parameter: an index. I will flip the bit from that index.
	*/
	b2.flip();
	TESTIFY_ASSERT(b2[0] == 1);
	TESTIFY_ASSERT(b2[1] == 1);
	TESTIFY_ASSERT(b2[2] == 1);
	TESTIFY_ASSERT(b2[3] == 1);
	TESTIFY_ASSERT(b2[4] == 1);
	TESTIFY_ASSERT(b2[5] == 1);
	TESTIFY_ASSERT(b2[6] == 1);
	TESTIFY_ASSERT(b2[7] == 1);

	b2.flip(4);
	TESTIFY_ASSERT(b2[4] == 0);

	/**
	* to_ulong function: converts the bitset to a unsigned long
	*/
	b2.reset();
	unsigned long val = b2.to_ulong();
	TESTIFY_ASSERT(val == 0);

	/**
	* to_string function: converts the bitset to a string
	*/
	std::string strVal = b2.to_string<char, std::char_traits<char>, std::allocator<char> >();
	TESTIFY_ASSERT(strVal == std::string("00000000") );

	/**
	* any function: returns whether any of the bits in the bitset is 1.
	*/
	TESTIFY_ASSERT(b2.any() == false);

	/**
	* none function: return whether none of the bits in the bitset is 1.
	*/
	TESTIFY_ASSERT(b2.none() == true);

	//bitset overloads the following operators:

	/**
	* operator=
	*/
	bitset<8> b2_copy;
	b2_copy = b2;

	/**
	* operator==
	*/
	TESTIFY_ASSERT(b2 == b2_copy);

	/**
	* operator!=
	*/
	b2_copy.flip(0);
	TESTIFY_ASSERT(b2 != b2_copy);

	/**
	* operator^= (bitwise exclusive-or operator)
	* a global operator^ is provided also
	*/
	bitset<4> lhsBitset(std::string("1001"));
	bitset<4> rhsBitset(std::string("0011"));
	lhsBitset ^= rhsBitset;							//1001 XOR 0011 => 1010
	bitset<4> expected_XOR_res(std::string("1010"));
	TESTIFY_ASSERT(lhsBitset == expected_XOR_res);

	/**
	* operator&= (AND operator)
	* a global operator & is provided also
	*/
	lhsBitset &= rhsBitset; 						// 1010 & 0011 => 0010
	bitset<4> expected_AND_res(std::string("0010"));
	TESTIFY_ASSERT(lhsBitset == expected_AND_res);

	/**
	* operator|= (OR operator)
	* a global operator | is provided also
	*/
	lhsBitset |= rhsBitset; 						// 0010 & 0011 => 0011
	bitset<4> expected_OR_res(std::string("0011"));
	TESTIFY_ASSERT(lhsBitset == expected_OR_res);

	/**
	* operator<<= (shift left)
	*/
	lhsBitset<<=1; 						// 0011<<1 => 0110
	bitset<4> expected_SL_res(std::string("0110"));
	TESTIFY_ASSERT(lhsBitset == expected_SL_res);

	/**
	* operator>>= (shift right)
	*/
	lhsBitset>>=1; 						// 0110>>1 => 0011
	bitset<4> expected_SR_res(std::string("0011"));
	TESTIFY_ASSERT(lhsBitset == expected_SR_res);

	/**
	* operator~ (NOT)
	*/
	~rhsBitset;		//~0011 => 1100
	bitset<4> expected_NOT_res(std::string("0011"));
	TESTIFY_ASSERT(rhsBitset == expected_NOT_res);
}

