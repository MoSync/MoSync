/*
Copyright (C) 2012 MoSync AB

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
 * @file Encoder.cpp
 * @author Mikael Kindborg
 *
 * Class for encoding/decoding data.
 */

#include <ma.h>				// MoSync API
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions
#include <MAUtil/String.h>

#include "Encoder.h"

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param str Encoded string.
	 * @return Decoded string.
	 */
	MAUtil::String Encoder::unescape(const MAUtil::String& str)
	{
		// The decoded string.
		MAUtil::String result = "";

		for (int i = 0; i < str.length(); ++i)
		{
			// If the current character is the '%' escape char...
			if ('%' == (char) str[i])
			{
				// Get the char value of the two digit hex value.
				MAUtil::String hex = str.substr(i + 1, 2);
				long charValue = strtol(
					hex.c_str(),
					NULL,
					16);
				// Append to result.
				result += (char) charValue;

				// Skip over the hex chars.
				i += 2;
			}
			else
			{
				// Not encoded, just copy the character.
				result += str[i];
			}
		}

		return result;
	}

	/**
	 * Take a string and "percent encode" it.
	 * @param str String to be encoded.
	 * @return Encoded string.
	 */
	MAUtil::String Encoder::escape(const MAUtil::String& str)
	{
		// The encoded string.
		MAUtil::String result = "";
		char buf[8];

		for (int i = 0; i < str.length(); ++i)
		{
			char c = str[i];
			if ((48 <= c && c <= 57) ||  // 0-9
				(65 <= c && c <= 90) ||  // a..z
				(97 <= c && c <= 122))   // A..Z
			{
				result.append(&str[i], 1);
			}
			else
			{
				result += "%";
				sprintf(buf, "%02X", str[i]);
				result += buf;
			}
		}

		return result;
	}

	/**
	 * Helper function.
	 */
	static void CharToHex(unsigned char c, char* hexBuf)
	{
		const char* hexchar = "0123456789ABCDEF";
		hexBuf[0] = hexchar[c >> 4];
		hexBuf[1] = hexchar[c & 0x0F];
	}

	/**
	 * Make a JSON stringified string.
	 * @param str String to be in JSON literal format.
	 * @return Stringified string.
	 */
	String Encoder::JSONStringify(const char* str)
	{
		String jsonString = "\"";

		char hexBuf[8];
		hexBuf[0] = '\\';
		hexBuf[1] = 'u';
		hexBuf[2] = '0';
		hexBuf[3] = '0';
		hexBuf[6] = 0;

		char strBuf[2];
		strBuf[1] = 0;

		int length = strlen(str);

		for (int i = 0; i < length; ++i)
		{
			strBuf[0] = str[i];
			String escaped = strBuf;
			switch (str[i])
			{
				case '\r': escaped = "\\r"; break;
				case '\n': escaped = "\\n"; break;
				case '\\': escaped = "\\\\"; break;
				case '/': escaped = "\\/"; break;
				case '"': escaped = "\\\""; break;
				case '\'': escaped = "\\'"; break;
				case '\f': escaped = "\\f"; break;
				case '\b': escaped = "\\b"; break;
				case '\t': escaped = "\\t"; break;
				default:
					if ((unsigned char) str[i] < 32)
					{
						CharToHex(str[i], hexBuf + 4);
						escaped = hexBuf;
					}
					break;
			}

			jsonString += escaped;
		}

		jsonString += "\"";

		return jsonString;
	}

	/**
	 * Encode data as base64.
	 * @param data Pointer to data to be encoded.
	 * @param length Number of bytes to be encoded.
	 * @return String object with encoded data.
	 */
	// Implementation is based on:
	// http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
	String Encoder::base64Encode(const void* data, int length)
	{
		const char* encodeLookup =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		const char padCharacter = '=';

		const char* dataPtr = (char*) data;
		String encodedString;
		int dataLength = length;
		int padCount = dataLength % 3;
		encodedString.reserve(((dataLength / 3) + (padCount > 0)) * 4);

		int i;
		unsigned long temp;

		for (i = 0; i < dataLength - 2; ++i)
		{
			// Convert to big endian.
			temp  = ((unsigned char)dataPtr[i])   << 16;
			temp += ((unsigned char)dataPtr[++i]) << 8;
			temp += ((unsigned char)dataPtr[++i]);

			encodedString += encodeLookup[(temp & 0x00FC0000) >> 18];
			encodedString += encodeLookup[(temp & 0x0003F000) >> 12];
			encodedString += encodeLookup[(temp & 0x00000FC0) >> 6 ];
			encodedString += encodeLookup[(temp & 0x0000003F)	  ];
		}

		switch (padCount)
		{
			case 1:
				// Convert to big endian.
				temp  = ((unsigned char)dataPtr[i]) << 16;
				encodedString += encodeLookup[(temp & 0x00FC0000) >> 18];
				encodedString += encodeLookup[(temp & 0x0003F000) >> 12];
				encodedString += padCharacter;
				encodedString += padCharacter;
				break;
			case 2:
				// Convert to big endian.
				temp  = ((unsigned char)dataPtr[i]) << 16;
				temp += ((unsigned char)dataPtr[++i]) << 8;
				encodedString += encodeLookup[(temp & 0x00FC0000) >> 18];
				encodedString += encodeLookup[(temp & 0x0003F000) >> 12];
				encodedString += encodeLookup[(temp & 0x00000FC0) >> 6 ];
				encodedString += padCharacter;
				break;
		}

		return encodedString;
	}

	/**
	 * Decode Base64 encoded data.
	 * @param data Pointer to zero-terminated string with encoded data.
	 * @return Pointer to decoded data buffer. NOTE: Deallocate this
	 * buffer using free().
	 *
	 *
	 */
	/**
	 * Decode Base64 encoded data. NOTE: The data buffer returned in
	 * the output parameter must be deallocated using free().
	 * @param input Pointer to zero-terminated string with encoded data.
	 * @param output Pointer to pointer that will point to the decoded data.
	 * @param input Pointer int that will contain the length of the decoded data.
	 * @return 1 on success, 0 on error.
	 */
	// Implementation is based on:
	// http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B_2
	int Encoder::base64Decode(const char* input, void** output, int* outputLength)
	{
		const char padCharacter = '=';

		char* encodedBytes = (char*) input;

		// Sanity checks.
		if (NULL == encodedBytes)
		{
			return 0; // Error
		}

		long inputLength = strlen(encodedBytes);

		if (0 == inputLength || inputLength % 4)
		{
			return 0; // Error
		}

		long padding = 0;
		if (encodedBytes[inputLength - 1] == padCharacter) { padding++; }
		if (encodedBytes[inputLength - 2] == padCharacter) { padding++; }

		// Allocate buffer to hold the result
		long decodedLength = ((inputLength / 4) * 3) - padding;
		char* decodedBytes = (char*) malloc(decodedLength);

		// Set output data.
		*output = decodedBytes;
		*outputLength = decodedLength;

		// Loop variables.
		long decodedIndex = 0;
		long temp = 0; // Holds decoded quanta
		int inputIndex = 0;
		while (inputIndex < inputLength)
		{
			// Process data in one base 64 quanta
			// (4 input bytes --> 3 output bytes).
			for (long pos = 0; pos < 4; ++pos)
			{
				// Input data.
				char ch = (char) encodedBytes[inputIndex];

				temp <<= 6;

				// This area will need tweaking if
				// you are using an alternate alphabet.
				if (ch >= 0x41 && ch <= 0x5A)
				{
					temp |= ch - 0x41;
				}
				else if (ch >= 0x61 && ch <= 0x7A)
				{
					temp |= ch - 0x47;
				}
				else if (ch >= 0x30 && ch <= 0x39)
				{
					temp |= ch + 0x04;
				}
				else if (ch == 0x2B)
				{
					temp |= 0x3E; // change to 0x2D for URL alphabet
				}
				else if (ch == 0x2F)
				{
					temp |= 0x3F; // change to 0x5F for URL alphabet
				}
				else if (ch == padCharacter)
				{
					// We have a pad character and are at the end
					// of the input.
					int numberOfPadCharacters = inputLength - inputIndex;
					if (1 == numberOfPadCharacters)
					{
						decodedBytes[decodedIndex++] = (temp >> 16) & 0x000000FF;
						decodedBytes[decodedIndex++] = (temp >> 8 ) & 0x000000FF;
						return 1; // Success
					}
					else if (2 == numberOfPadCharacters)
					{
						decodedBytes[decodedIndex++] = (temp >> 10) & 0x000000FF;
						return 1; // Success
					}
					else
					{
						// Invalid padding.
						free(decodedBytes);
						return 0; // Error
					}
				}
				else
				{
					// Invalid Base64 character.
					free(decodedBytes);
					return 0; // Error
				}

				// Increment index in input data.
				++inputIndex;
			} // for

			decodedBytes[decodedIndex++] = (temp >> 16) & 0x000000FF;
			decodedBytes[decodedIndex++] = (temp >> 8 ) & 0x000000FF;
			decodedBytes[decodedIndex++] = (temp	  ) & 0x000000FF;
		} // while

		return 1; // Success
	}

} // namespace
