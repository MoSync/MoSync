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

#include "Utils.h"
#include <vector>
#include <string>


namespace Utils
{
	// StringCopy
	//-------------------------------------------------------------------------
/**
 * \brief stringCopy, copy a subset of a string, with optinal trim parameters.
 * @param in	,input string
 * @param start	,start from index
 * @param end	,end at index
 * @param trim	,trim spaces if flag == true (removes all spaces in string, by brute force)
 * @return 		,output string result
 */
	std::string stringCopy(std::string &in, size_t start, size_t end, bool trim)
	{
		std::string value;
		for(size_t i=start; i<end; ++i)
		{
			if (trim && in[i] == ' ')
				continue;
			value.push_back(in[i]);
		}
		return value;
	}

	// Spilt function, like from boost, perl, Java etc.
	//-------------------------------------------------------------------------
/**
 * \brief split		,tokenizer that splits string up into vectored string sing delimiter and/or trimming text
 * @param line		,one row of data untill EOL
 * @param delimiter ,using delimiter to split substrings left right side
 * @param trim		,optional parameter for removing spaces from string
 * @return vector of strings that has been splitted up with the delimiter.
 */
	std::vector<std::string> split(std::string &line,char delimiter,bool trim)
	{
		size_t len = line.size();
		std::vector<std::string> lineData;
		for(size_t start = 0; start<len;)
		{
			size_t end=start;
			for(; end<=len && (line[end] != delimiter); ++end);		// search for first space

			if (!(start == end && line[end] == delimiter))
				lineData.push_back(stringCopy(line,start,end,trim));	// only store something that starts with != delim otherwise skip entry

			start = end+1;	//update next entry
		}
		return lineData;
	}

	// Un Quote a string (remove its quotes)
	//-----------------------------------------------------------
/**
 * \brief unQuote	,removes quotes from a string e.g. "test123" => test123
 * @param str		,input string
 * @param quote		,quote char e.g. "
 * @return output unquoted string
 */
	std::string unQuote(const std::string &str,const char quote)
	{
		std::string out;
		// out.reserve(str.size());
		for(size_t i=0; i<str.size(); i++)
		{
			char c = str[i];
			if (c == quote)
				continue;
			out.push_back(c);
		}
		return out;
	}

}
