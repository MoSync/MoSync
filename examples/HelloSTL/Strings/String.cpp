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
#include <assert.h>
#include "../STLMoblet.h"
#include "../Log.h"

/**
*  std::string is a template class designed to manipulate sequences of characters.
*  It is a container of characters.
*  The string class is defined in the <string> header.
*/
void STLMoblet::STL_string()
{
	using std::string;


	LOG("\n");
	LOG("========================= string ================================================================================");
	log_to_console("\n    std::string is a template class designed to manipulate "
					"sequences of characters."
					"\n    It is a container of characters."
					"\n    The string class is defined in the <string> header.");


	log_to_console(	 "\n    /**"
					 "\n    *  default constructor"
					 "\n    */");
	std::string emptyString;
	log_to_console("std::string emptyString;");

	log_to_console( "\n    /**"
					"\n    *  empty: returns true if the string is empty (doesn't contain any characters)"
					"\n    */");
	bool isEmpty = emptyString.empty();
	log_to_console(isEmpty, "emptyString.empty();	//returns:");


	log_to_console(
	"\n    /**constructor"
	"\n    * string provides several ways to construct a string:"
	"\n    \n    * string ( const string& arg );"
	"\n    * Constructs a string from another string."
	"\n    * "
	"\n    * string ( const string& arg, size_t position, size_t n = npos );"
	"\n    * Creates a string by copying \"n\" characters, starting with"
	"\"position\", from \"arg\". If \"n\" has it's default value( npos ),"
	"all the characters, from \"position\" to end are copied."
	"\n    * "
	"\n    * string ( const char * array, size_t n );"
	"\n    * Constructs a string from the first \"n\" characters of the \"array\"."
	"\n    * "
	"\n    * string ( const char * array );"
	"\n    * Constructs a string with an array of chars."
	"\n    * "
	"\n    * string ( size_t n, char c );"
	"\n    * Creates a string containing \"n\" number of \"c\" characters.**/");


	string myFirstString("Hello! How are you?");
	log_to_console("\n     string myFirstString(\"Hello! How are you?\");");

	string salutation(myFirstString, 0, 6);
	log_to_console(salutation.c_str(), "string salutation(myFirstString, 0, 6);		"
			"//copies 6 characters, from beginning. salutation will contain: ");


	string sst(4, 's');
	log_to_console(sst.c_str(), "\n     string sst(4, 's');	//sst will contain: ");


	log_to_console( "\n    /**"
					"\n    *  operator+="
					"\n    */");
	sst += 't';
	log_to_console(sst.c_str(), "\n     sst += 't';	//sst will contain:");

	string reason = "! It's a meeting here.";
	log_to_console("\n     string reason = \"! It's a meeting here.\";");

	sst += reason;
	log_to_console(sst.c_str(), "sst += reason;	//sst will contain: ");


	log_to_console(
	"\n    /**insert: inserts a string or a character into a string, at a specified position."
	"\n    std::insert has several overloads:"
	"\n    *    string& insert ( size_t position,  const string& arg );"
	"\n    *    Inserts the string \"arg\", at \"position\"."
	"\n    *    "
	"\n    *    string& insert ( size_t position1, const string& arg, size_t position2, size_t n );"
	"\n    *    Inserts, at \"position1\", \"n\" characters from \"arg\", starting with \"position2\"."
	"\n    *    "
	"\n    *    string& insert ( size_t position, const char* array, size_t n );"
	"\n    *    Inserts at \"position\" \"n\" characters from \"array\"."
	"\n    *    "
	"\n    *    string& insert ( size_t position,  const char* array );"
	"\n    *    Inserts at \"position\" all the characters from \"array\"."
	"\n    *    "
	"\n    *    string& insert ( size_t position, size_t n, char c );"
	"\n    *    Inserts at \"position\" \"n\" characters of \"c\"."
	"\n    *    "
	"\n    *    iterator insert ( iterator it, char c );"
	"\n    *    Inserts at the location pointed by \"it\" the character \"c\"."
	"\n    *    "
	"\n    *    void insert ( iterator it, size_t n, char c );"
	"\n    *    Inserts at the location pointed by \"it\" n characters of \"c\"."
	"\n    *    "
	"\n    *    void insert ( iterator it, InputIterator first, InputIterator last );"
	"\n    *    Inserts at the location pointed by \"it\", all the characters in the range [first, last).*/"
	"\n    *    ");

	sst.insert(6 ," See?");
	log_to_console(sst.c_str(), "\n    sst.insert(6 ,\" See?\");	//sst will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  replace: replaces a part of a string"
	"\n     *  prototype: string& replace ( size_t position, size_t n,   const string& arg );"
	"\n     *  replaces \"n\" characters, starting with position \"position\", with the string \"arg\"."
	"\n     */");

	string replaceWith = "Everybody is sleeping.";
	string replaced = "It's a meeting here.";
	int position = sst.find(replaced);
	sst.replace(position, replaced.size(), replaceWith);

	log_to_console("string replaceWith = \"Everybody is sleeping.\";");
	log_to_console("string replaced = \"It's a meeting here.\";");
	log_to_console(position, "int position = sst.find(replaced);	//position = ");
	log_to_console(sst.c_str(), "sst.replace(position, replaced.size(), replaceWith);	//sst will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  erase: erases a part of a string. The size of the string will be shortened."
	"\n     *  string& erase ( size_t position = 0, size_t n = npos );"
	"\n     *  erases \"n\" characters, starting with \"position\". If the default values are used (0, npos), all the characters from the string will be erased."
	"\n     */");

	string eraseThis = "sleeping.";
	position = sst.find(eraseThis);
	sst.erase(position, eraseThis.size());

	log_to_console("string eraseThis = \"sleeping.\";");
	log_to_console(position, "position = sst.find(eraseThis);		//position = ");
	log_to_console(sst.c_str(), "sst.erase(position, eraseThis.size());	//sst will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  assign: appends to a string"
	"\n     */");

	sst.append("listening.");
	log_to_console(sst.c_str(), "sst.append(\"listening.\");	//sst will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  swap: swaps content with another string."
	"\n     */");

	string answer("What's this?????");
	log_to_console(answer.c_str(), "string answer(\"What's this?????\");");

	answer.swap(sst);
	log_to_console(sst.c_str(), "answer.swap(sst);	//sst will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  string is actually a special container that holds and manipulates chars. So it has iterators, "
	"\n     *  provides a operator[], size(), resize(), clear() and other functions, similar to the other containers from STL."
	"\n     */");

	string str = "STL";
	log_to_console("string str = \"STL\";");

	//std::string::iterator it = str.begin();
	log_to_console("std::string::iterator it = str.begin();");

	for(std::string::iterator it = str.begin(); it!= str.end(); ++it)
	{
		*it = 'A';
	}

	log_to_console("for( std::string::iterator it = str.begin(); it!= str.end(); ++it)");
	log_to_console("{");
	log_to_console("	*it = 'A';");
	log_to_console("}");
	log_to_console(str.c_str(), "str contains now: ");


	log_to_console(
	"\n     /**"
	"\n     *  clear: erases the content of a string. Results an empty string."
	"\n     */");

	str.clear();
	log_to_console(str.empty(), "str.clear();	//str.empty() returns: ");


	log_to_console(
	"\n     /**"
	"\n     *  c_str: returns a const char* pointer to an NULL terminated array of characters, with the same content as the string."
	"\n     */");

	string learn = "C++";
	char *chars = new char[learn.size() + 1];
	log_to_console("string learn = \"C++\";");
	log_to_console(chars, "char *chars = new char[learn.size() + 1];	//chars contains: ");

	const char *strChars = learn.c_str();
	strcpy(chars, strChars);
	log_to_console("const char *strChars = learn.c_str();");
	log_to_console(chars, learn.size() + 1, "strcpy(chars, strChars);	//chars will contain: ");

	delete []chars;
	log_to_console("delete []chars;");


	log_to_console(
	"\n     /**"
	"\n     *  copy: copies a number of characters from a string to an array, starting from a specified index."
	"\n     *  prototype: size_t copy ( char* array, size_t n, size_t position = 0) const;"
	"\n     *  string::copy copies into the \"array\", \"n\" characters starting from \"position\"."
	"\n     */");

	char array[5];
	log_to_console(array, 5, "char array[5];	//array contains: ");

	string hello = "Hello world!";
	hello.copy(array, 5, 0);

	log_to_console("string hello = \"Hello world!\";");
	log_to_console(array, 5, "hello.copy(array, 5, 0);	//array will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  find: searches for a string, an array or a character inside a string. Returns the position"
	"\n     *  were it finds the first occurrence of it, or string::npos, if it didn't found anything."
	"\n     *  string::copy copies into the \"array\", \"n\" characters starting from \"position\"."
	"\n     *  It has four overloads:"
	"\n     *  		size_t find ( const string& arg, size_t position = 0 ) const;"
	"\n     *  		size_t find ( const char* array, size_t position, size_t n ) const;"
	"\n     *  		size_t find ( const char* array, size_t position = 0 ) const;"
	"\n     *  		size_t find ( char c, size_t position = 0 ) const;"
	"\n     */");

	string someWords = "I don't know what to say.";
	int found = someWords.find("say");
	log_to_console("string someWords = \"I don't know what to say.\";");
	log_to_console(found, "int found = someWords.find(\"say\");	\\found = ");

	found = someWords.find("Not there");
	assert("found == std::string::npos");
	log_to_console("found = someWords.find(\"Not there\");	//found == string::npos");

	found = someWords.find('I');
	log_to_console(found, "found = someWords.find('I');	//found = 0");


	log_to_console(
	"\n     /**"
	"\n     *  rfind: searches for a string, an array or a character inside a string. Returns the position"
	"\n     *  were it finds the first occurrence of it, or string::npos, if it didn't found anything."
	"\n     *  It has four overloads:."
	"\n     *  		size_t rfind ( const string& arg, size_t position = npos ) const;"
	"\n     *  		size_t rfind ( const char* array, size_t position, size_t n ) const;"
	"\n     *  		size_t rfind ( const char* array, size_t position, size_t n ) const;"
	"\n     *		size_t rfind ( char c, size_t position = npos ) const;"
	"\n     */");

	string words = "Go on and on and on and on, until you finish.";
	int rfound = words.rfind("on");
	log_to_console("string words = \"Go on and on and on and on, until you finish.\";");
	log_to_console(rfound, "int rfound = words.rfind(\"on\");	//rfound = ");

	found = words.find("on");
	log_to_console(found, "found = words.find(\"on\");	//found = ");


	log_to_console(
	"\n     /**"
	"\n     *  find_first_of: finds the first occurrence of any of the characters that are part of some"
	"\n     *  string, supplied as argument. We can supply also an array of characters, instead of a string."
	"\n     *  It has also an overload version for searching the first occurrence of character."
	"\n     *  We can specify the position from which the search is performed. The default value is 0."
	"\n     *  If it doesn't find anything returns string::npos. Otherwise returns the position were it found"
	"\n     *  any of the characters searched."
	"\n     *  It has four overloads:"
	"\n     *  	size_t find_first_of ( const string& arg, size_t position = 0 ) const;"
	"\n     *		size_t find_first_of ( const char* array, size_t position, size_t n ) const;"
	"\n     *		size_t find_first_of ( const char* s, size_t position = 0 ) const;"
	"\n     *		size_t find_first_of ( char c, size_t position = 0 ) const;"
	"\n     */");

	string someString = "yes, yes I am a string.";
	log_to_console("string someString = \"yes, yes I am a string.\"");

	found = someString.find_first_of('e');
	log_to_console(found, "found = someString.find_first_of('e');	//found = ");

	found = someString.find_first_of("yes");
	log_to_console(found, "found = someString.find_first_of(\"yes\");	//found = ");

	found = someString.find_first_of('e', 2);
	log_to_console(found, "found = someString.find_first_of('e', 2);	//found = ");

	found = someString.find_first_of("abc");
	log_to_console(found, "found = someString.find_first_of(\"abc\");	//found = string::npos");


	log_to_console(
	"\n     /**"
	"\n     *  find_last_of: finds the last occurrence of any of the characters that are part of some"
	"\n     *  string, supplied as argument. We can supply also an array of characters, instead of a string."
	"\n     *  It has also an overload version for searching the last occurrence of a character."
	"\n     *  We can specify the position from which the search is performed. The default value is 0."
	"\n     *  If it doesn't find anything returns string::npos. Otherwise returns the position were it found"
	"\n     *  any of the characters searched."
	"\n     *  It has four overloads:"
	"\n     *  	size_t find_last_of ( const string& arg, size_t position = 0 ) const;"
	"\n     *		size_t find_last_of ( const char* array, size_t position, size_t n ) const;"
	"\n     *		size_t find_last_of ( const char* s, size_t position = 0 ) const;"
	"\n     *		size_t find_last_of ( char c, size_t position = 0 ) const;"
	"\n     */");

	found = someString.find_last_of('e');
	log_to_console(found, "found = someString.find_last_of('e');	//someString = \"yes, yes I am a string.\"; found = ");

	found = someString.find_last_of("yes");
	log_to_console(found, "found = someString.find_last_of(\"yes\");	//found = ");

	found = someString.find_first_of('e', 16);
	log_to_console(found, "found = someString.find_first_of('e', 16);	//found == string::npos; found = ");

	found = someString.find_last_of("abc");
	log_to_console("\n     //find the last occurrence of any of the a, b, or c chars in the string \"yes, yes I am a string.\"");
	log_to_console(found, "found = someString.find_last_of(\"abc\");	//found = ");


	log_to_console(
	"\n     /**"
	"\n     *  find_first_not_of: finds the first occurrence of any of the characters that are not part of some"
	"\n     *  string, supplied as argument. We can supply also an array of characters, instead of a string."
	"\n     *  It has also an overload version for searching the first character that is different from a character"
	"\n     *  provided as argument."
	"\n     *  If it doesn't find anything returns string::npos. Otherwise returns the position of the first"
	"\n     *  character different from the ones provided as argument."
	"\n     *  It has four overloads:"
	"\n     *  		size_t find_first_not_of ( const string& str, size_t position = 0 ) const;"
	"\n     *		size_t find_first_not_of ( const char* s, size_t position, size_t n ) const;"
	"\n     *		size_t find_first_not_of ( const char* s, size_t position = 0 ) const;"
	"\n     *		size_t find_first_not_of ( char c, size_t position = 0 ) const;"
	"\n     */");

	found = someString.find_first_not_of('e');
	log_to_console(found, "found = someString.find_first_not_of('e');	//someString = \"yes, yes I am a string.\"; found = ");


	found = someString.find_first_not_of("yes");
	log_to_console(found, "found = someString.find_first_not_of(\"yes\");	//found = ");


	log_to_console(
	"\n     /**"
	"\n     *  find_last_not_of: finds the last occurrence of any of the characters that are not part of some"
	"\n     *  string, supplied as argument. We can supply also an array of characters, instead of a string."
	"\n     *  It has also an overload version for searching the last character that is different from a character"
	"\n     *  provided as argument."
	"\n     *  It has four overloads:"
	"\n     *  character different from the ones provided as argument."
	"\n     *  It has four overloads:"
	"\n     *  	size_t find_last_not_of ( const string& arg, size_t position = 0 ) const;"
	"\n     *		size_t find_last_not_of ( const char* array, size_t position, size_t n ) const;"
	"\n     *		size_t find_last_not_of ( const char* s, size_t position = 0 ) const;"
	"\n     *		size_t find_last_not_of ( char c, size_t position = 0 ) const;"
	"\n     *  If it doesn't find anything returns string::npos. Otherwise returns the position of the last"
	"\n     *  character different from the ones provided as argument."
	"\n     */");

	found = someString.find_last_not_of('e');
	log_to_console(found, "found = someString.find_last_not_of('e');	"
			"//someString = \"yes, yes I am a string.\"; "
			"\n    \t\t\t\t\t\t find_last_not_of will return the position of the last character from the string ('.');"
			"\n    \t\t\t\t\t\t found = ");


	found = someString.find_last_not_of("yes");
	log_to_console(found, "found = someString.find_last_not_of(\"yes\");	//found = ");


	log_to_console(
	"\n     /**"
	"\n     *  substr: returns a part of a string."
	"\n     *  prototype: string substr ( size_t position = 0, size_t n = npos ) const;"
	"\n     *  substr returns the string that starts at \"position\" and contains the next \"n\" characters."
	"\n     */");

	string letters = "ABCDEFGHIJKLM";
	log_to_console("string letters = \"ABCDEFGHIJKLM\";");

	log_to_console("\n    //extract the first two letters");
	string ab = letters.substr(0, 2);
	log_to_console(ab.c_str(), "string ab = letters.substr(0, 2);	// ab will contain: ");

	log_to_console("\n    //extract three letters starting from the third letter:");
	string cde = letters.substr(2, 3);
	log_to_console(cde.c_str(), "string cde = letters.substr(2, 3);	//cde will contain: ");

	log_to_console("\n    //extract all the letters, starting with the third letter:");
	string all = letters.substr(2);
	log_to_console(all.c_str(), "string all = letters.substr(2);	//all will contain: ");


	log_to_console(
	"\n     /**"
	"\n     *  string overloads several operators, for comparing strings"
	"\n     *  The overloaded operators are: ==, !=, >, >=, <, <="
	"\n     */");

	string first = "Aha! Now I get it.";
	string second = "Really?";

	log_to_console("string first = \"Aha! Now I get it.\";");
	log_to_console("string second = \"Really?\";");

	first.swap(second);
	log_to_console(first.c_str(), "first.swap(second);	//first will contain:: ");
	log_to_console(second.c_str(), "\t\t\t\t second will contain: ");

	first = second;
	log_to_console(first.c_str(), "\n    first = second;	//first will contain: ");

}
