/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>

#if defined(MOSYNC_NATIVE) && defined(__IOS__)
#include <ext/hash_map>

namespace std {
	using namespace __gnu_cxx;
}

namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& x ) const
		{
			return hash< const char* >()( x.c_str() );
		}
	};
}

#else
#include <hash_map>
#endif

/**
 * \brief namespace Utils	, small global functions encapsulated under the namespace Utils
 */
namespace Utils
{
	/**
	 * \brief stringCopy, copy a subset of a string, with optinal trim parameters.
	 * @param in	,input string
	 * @param start	,start from index
	 * @param end	,end at index
	 * @param trim	,trim spaces if flag == true (removes all spaces in string, by brute force)
	 * @return 		,output string result
	 */

	std::vector<std::string> split(std::string &line,char delimiter,bool trim);

	/**
	 * \brief split		,tokenizer that splits string up into vectored string sing delimiter and/or trimming text
	 * @param line		,one row of data untill EOL
	 * @param delimiter ,using delimiter to split substrings left right side
	 * @param trim		,optional parameter for removing spaces from string
	 * @return vector of strings that has been splitted up with the delimiter.
	 */

	std::string stringCopy(std::string &in, size_t start, size_t end, bool trim);

	/**
	 * \brief unQuote	,removes quotes from a string e.g. "test123" => test123
	 * @param str		,input string
	 * @param quote		,quote char e.g. "
	 * @return output unquoted string
	 */

	std::string unQuote(const std::string &str,const char quote= '\"');
}

#endif /* UTILS_H_ */
