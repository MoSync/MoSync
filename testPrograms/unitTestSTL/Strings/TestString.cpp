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

/**
 * @file TestString.cpp
 * @author Gabriela Rata
 */

#include <string>
#include "../TestSTL.h"
#include "../Log.h"

/**
*  std::string is a template class designed to manipulate sequences of characters.
*  It is a container of characters.
*  the string class is defined in the <string> header.
*/
void TestSTL::test_string()
{
	using std::string;

	/**
	*  default constructor
	*/
	std::string emptyString;

	/**
	*  empty: returns true if the string is empty (doesn't contain any characters)
	*/
	TESTIFY_ASSERT(emptyString.empty() == true);

	/**
	*  constructor
	*  string provides several ways to construct a string:
	*  		string ( const string& arg );
	*  		Constructs a string from another string.
	*
	*  		string ( const string& arg, size_t position, size_t n = npos );
	*  		Creates a string by copying "n" characters, starting with "position", from "arg". If "n"
	*  		has it's default value( npos ), all the characters, from "position" to end are copied.
	*
	*  		string ( const char * array, size_t n );
	*  		Constructs a string from the first "n" characters of the "array".
	*
	*  		string ( const char * array );
	*  		Constructs a string with an array of chars.
	*
	*  		string ( size_t n, char c );
	*  		Creates a string containing "n" number of "c" characters.
	*/
	string myFirstString("Hello! How are you?");

	string salutation(myFirstString, 0, 6); //copies 6 characters, from beginning => "Hello!"
	TESTIFY_ASSERT(salutation == "Hello!");

	string sst(4, 's');
	TESTIFY_ASSERT(sst == "ssss");

	/**
	*  operator+=
	*/
	sst += 't';
	TESTIFY_ASSERT(sst == "sssst");

	string reason = "! It's a meeting here.";
	sst += reason;
	TESTIFY_ASSERT(sst == "sssst! It's a meeting here.");

	/**
	*  insert: inserts a string or a character into a string, at a specified position.
	*  std::insert has several overloads:
	* 		string& insert ( size_t position,  const string& arg );
	* 		Inserts the string "arg", at "position".
	*
	* 		string& insert ( size_t position1, const string& arg, size_t position2, size_t n );
	* 		Inserts, at "position1", "n" characters from "arg", starting with "position2".
	*
	* 	 	string& insert ( size_t position, const char* array, size_t n );
	* 		Inserts at "position" "n" characters from "array".
	*
	* 	 	string& insert ( size_t position,  const char* array );
	*		Inserts at "position" all the characters from "array".
	*
	* 	 	string& insert ( size_t position, size_t n, char c );
	* 		Inserts at "position" "n" characters of "c".
	*
	*		iterator insert ( iterator it, char c );
	*		Inserts at the location pointed by "it" the character "c".
	*
	*		void insert ( iterator it, size_t n, char c );
	*		Inserts at the location pointed by "it" n characters of "c".
	*
	*		void insert ( iterator it, InputIterator first, InputIterator last );
	*		Inserts at the location pointed by "it", all the characters in the range [first, last).
	*/

	sst.insert(6 ," See?");
	TESTIFY_ASSERT(sst == "sssst! See? It's a meeting here.");

	/**
	*  replace: replaces a part of a string
	*  prototype: string& replace ( size_t position, size_t n,   const string& arg );
	*  replaces "n" characters, starting with position "position", with the string "arg".
	*/
	string replaceWith = "Everybody is sleeping.";
	string replaced = "It's a meeting here.";
	int position = sst.find(replaced);
	sst.replace(position, replaced.size(), replaceWith);
	TESTIFY_ASSERT(sst == "sssst! See? Everybody is sleeping.");

	/**
	*  erase: erases a part of a string. The size of the string will be shortened.
	*  string& erase ( size_t position = 0, size_t n = npos );
	*  erases "n" characters, starting with "position". If the default values are used (0, npos),
	*  all the characters from the string will be erased.
	*/
	string eraseThis = "sleeping.";
	position = sst.find(eraseThis);
	sst.erase(position, eraseThis.size());
	TESTIFY_ASSERT(sst == "sssst! See? Everybody is "); //	listening

	/**
	*  assign: appends to a string
	*/
	sst.append("listening.");
	TESTIFY_ASSERT(sst == "sssst! See? Everybody is listening.");

	/**
	*  swap: swaps content with another string.
	*/
	string answer("What's this?????");
	answer.swap(sst);
	TESTIFY_ASSERT( answer == "sssst! See? Everybody is listening.");
	TESTIFY_ASSERT( sst == "What's this?????" );

	/**
	*  string is actually a special container that holds and manipulates chars. So it has iterators,
	*  provides a operator[], size(), resize(), clear() and other functions, similar to the other
	*  containers from STL.
	*/
	string str = "STL";
	TESTIFY_ASSERT(str.size() == 3);
	std::string::iterator it = str.begin();
	for( std::string::iterator it = str.begin(); it!= str.end(); ++it)
	{
		*it = 'A';
	}
	TESTIFY_ASSERT( str[0] == 'A' && str[1]=='A' && str[2] == 'A');

	/**
	*  clear: erases the content of a string. Results an empty string.
	*/
	str.clear();
	TESTIFY_ASSERT( str.empty() == true );

	/**
	*  c_str: returns a const char* pointer to an NULL terminated array of characters, with the same
	*  content as the string.
	*/
	string learn = "C++";
	char *chars = new char[learn.size() + 1];

	const char *strChars = learn.c_str();
	strcpy(chars, strChars);
	TESTIFY_ASSERT( (chars[0] == 'C' && strChars[0] == 'C') &&
					(chars[1] == '+' && strChars[1] == '+') &&
					(chars[2] == '+' && strChars[2] == '+') &&
					(chars[3] == '\0' && strChars[3] == '\0') );
	delete []chars;

	/**
	*  copy: copies a number of characters from a string to an array, starting from a specified index.
	*  prototype: size_t copy ( char* array, size_t n, size_t position = 0) const;
	*  string::copy copies into the "array", "n" characters starting from "position".
	*/
	char array[5];
	string hello = "Hello world!";
	hello.copy(array, 5, 0);
	TESTIFY_ASSERT(	array[0] == 'H' &&
					array[1] == 'e' &&
					array[2] == 'l' &&
					array[3] == 'l' &&
					array[4] == 'o' );

	/**
	*  find: searches for a string, an array or a character inside a string. Returns the position
	*  were it finds the first occurrence of it, or string::npos, if it didn't found anything.
	*  It has four overloads:
	*  		size_t find ( const string& arg, size_t position = 0 ) const;
	*  		size_t find ( const char* array, size_t position, size_t n ) const;
	*  		size_t find ( const char* array, size_t position = 0 ) const;
	*  		size_t find ( char c, size_t position = 0 ) const;
	*/
	string someWords = "I don't know what to say.";
	int found = someWords.find("say");
	TESTIFY_ASSERT( found != string::npos );

	found = someWords.find("Not there");
	TESTIFY_ASSERT( found == string::npos );

	found = someWords.find('I');
	TESTIFY_ASSERT( 0 == found );

	/**
	*  rfind: searches for a string, an array or a character inside a string. Returns the position
	*  were it finds the first occurrence of it, or string::npos, if it didn't found anything.
	*  It has four overloads:
	*  		size_t rfind ( const string& arg, size_t position = npos ) const;
	*  		size_t rfind ( const char* array, size_t position, size_t n ) const;
	*		size_t rfind ( const char* array, size_t position = npos ) const;
	*		size_t rfind ( char c, size_t position = npos ) const;
	*/
	string words = "Go on and on and on and on, until you finish.";
	int rfound = words.rfind("on");
	TESTIFY_ASSERT( rfound != string::npos);

	found = words.find("on");
	TESTIFY_ASSERT( found != string::npos && rfound > found );

	/**
	*  find_first_of: finds the first occurrence of any of the characters that are part of some
	*  string, supplied as argument. We can supply also a array of characters, instead of a string.
	*  It has also an overload version for searching the first occurrence of character.
	*  We can specify the position from which the search is performed. The default value is 0.
	*  If it doesn't find anything returns string::npos. Otherwise returns the position were it found
	*  any of the characters searched.
	*  It has four overloads:
	*  		size_t find_first_of ( const string& arg, size_t position = 0 ) const;
	*		size_t find_first_of ( const char* array, size_t position, size_t n ) const;
	*		size_t find_first_of ( const char* s, size_t position = 0 ) const;
	*		size_t find_first_of ( char c, size_t position = 0 ) const;
	*/
	//				  	 0123456789
	string someString = "yes, yes I am a string.";

	found = someString.find_first_of('e');
	TESTIFY_ASSERT( 1 == found );

	found = someString.find_first_of("yes");
	TESTIFY_ASSERT( 0 == found );

	found = someString.find_first_of('e', 2);
	TESTIFY_ASSERT( 6 == found );

	found = someString.find_first_of("abc");
	TESTIFY_ASSERT( found != string::npos );
	TESTIFY_ASSERT( someString[found] == 'a' && someString[found+1] == 'm');

	/**
	*  find_last_of: finds the last occurrence of any of the characters that are part of some
	*  string, supplied as argument. We can supply also an array of characters, instead of a string.
	*  It has also an overload version for searching the last occurrence of a character.
	*  We can specify the position from which the search is performed. The default value is 0.
	*  If it doesn't find anything returns string::npos. Otherwise returns the position were it found
	*  any of the characters searched.
	*  It has four overloads:
	*  		size_t find_first_of ( const string& arg, size_t position = 0 ) const;
	*		size_t find_first_of ( const char* array, size_t position, size_t n ) const;
	*		size_t find_first_of ( const char* s, size_t position = 0 ) const;
	*		size_t find_first_of ( char c, size_t position = 0 ) const;
	*/
	found = someString.find_last_of('e');  		//someString = "yes, yes I am a string.";
	TESTIFY_ASSERT( 6 == found );

	found = someString.find_last_of("yes");		//someString = "yes, yes I am a string." => returns 16, which is the beginning of "string.".
	TESTIFY_ASSERT( found == 16 &&
					someString[found] 	== 's' &&
					someString[found+1] == 't' &&
					someString[found+2] == 'r' &&
					someString[found+3] == 'i' &&
					someString[found+4] == 'n' &&
					someString[found+5] == 'g'	);

	found = someString.find_first_of('e', 16);
	TESTIFY_ASSERT( string::npos == found );

	found = someString.find_last_of("abc");		//find the last occurrence of any of the a, b, or c chars in the string "yes, yes I am a string.";
	TESTIFY_ASSERT( found != string::npos );
	TESTIFY_ASSERT( someString[found] == 'a' && someString[found+1] == ' ' && someString[found+2] == 's');

	/**
	*  find_first_not_of: finds the first occurrence of any of the characters that are not part of some
	*  string, supplied as argument. We can supply also an array of characters, instead of a string.
	*  It has also an overload version for searching the first character that is different from a character
	*  provided as argument.
	*  If it doesn't find anything returns string::npos. Otherwise returns the position of the first
	*  character different from the ones provided as argument.
	*  It has four overloads:
	*  		size_t find_first_not_of ( const string& str, size_t position = 0 ) const;
	*		size_t find_first_not_of ( const char* s, size_t position, size_t n ) const;
	*		size_t find_first_not_of ( const char* s, size_t position = 0 ) const;
	*		size_t find_first_not_of ( char c, size_t position = 0 ) const;
	*/
	found = someString.find_first_not_of('e');  //someString = "yes, yes I am a string.";
	TESTIFY_ASSERT ( someString[found] == 'y' );

	found = someString.find_first_not_of("yes");
	TESTIFY_ASSERT ( someString[found] == ',' );

	/**
	*  find_last_not_of:
	*/
	/**
	*  find_last_not_of: finds the last occurrence of any of the characters that are not part of some
	*  string, supplied as argument. We can supply also an array of characters, instead of a string.
	*  It has also an overload version for searching the last character that is different from a character
	*  provided as argument.
	*  It has four overloads:
	*  		size_t find_last_not_of ( const string& arg, size_t position = 0 ) const;
	*		size_t find_last_not_of ( const char* array, size_t position, size_t n ) const;
	*		size_t find_last_not_of ( const char* s, size_t position = 0 ) const;
	*		size_t find_last_not_of ( char c, size_t position = 0 ) const;
	*  If it doesn't find anything returns string::npos. Otherwise returns the position of the last
	*  character different from the ones provided as argument.
	*/
	found = someString.find_last_not_of('e');  		//someString = "yes, yes I am a string.";
	TESTIFY_ASSERT ( someString[found] == '.' );	//returns the position of the last character from the string

	found = someString.find_last_not_of("yes");
	TESTIFY_ASSERT ( someString[found] == '.' );

	/**
	*  substr: returns a part of a string.
	*  prototype: string substr ( size_t position = 0, size_t n = npos ) const;
	*  substr returns the string that starts at "position" and contains the next "n" characters.
	*/
	string letters = "ABCDEFGHIJKLM";

	//extract the first two letters
	string ab = letters.substr(0, 2);
	TESTIFY_ASSERT(ab == "AB");

	//extract three letters starting with "C"
	string cde = letters.substr(2, 3);	//A->0, B->1, C->2
	TESTIFY_ASSERT( cde == "CDE");

	//extract all the letters, starting with "C"
	string all = letters.substr(2);
	TESTIFY_ASSERT( all == "CDEFGHIJKLM");

	/**
	*  string overloads several operators, for comparing strings
	*  The overloaded operators are: ==, !=, >, >=, <, <=
	*/
	string first = "Aha! Now I get it.";
	string second = "Really?";

	TESTIFY_ASSERT( first != second );
	TESTIFY_ASSERT( first < second );
	first.swap(second);
	TESTIFY_ASSERT( first > second );

	first = second;
	TESTIFY_ASSERT( first == second );
	TESTIFY_ASSERT( first >= second );
	TESTIFY_ASSERT( first <= second );
}
