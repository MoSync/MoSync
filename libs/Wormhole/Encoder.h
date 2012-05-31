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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

 /**
 * @file Encoder.h
 * @author Mikael Kindborg
 *
 * @brief Class for encoding/decoding data.
 */

#ifndef WORMHOLE_ENCODER_H_
#define WORMHOLE_ENCODER_H_

#include <ma.h>
#include <MAUtil/String.h>

namespace Wormhole
{

/**
 * @brief Class with methods to encode/decode data
 * as strings.
 */
class Encoder
{
public:
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param str Encoded string.
	 * @return Decoded string.
	 */
	static MAUtil::String unescape(const MAUtil::String& str);

	/**
	 * Take a string and "percent encode" it.
	 * @param str String to be encoded.
	 * @return Encoded string.
	 */
	static MAUtil::String escape(const MAUtil::String& str);

	/**
	 * Make a JSON stringified string.
	 * @param str String to be in JSON literal format.
	 * @return Stringified string.
	 */
	static MAUtil::String JSONStringify(const char* str);

	/**
	 * Encode data as base64.
	 * @param data Pointer to data to be encoded.
	 * @param length Number of bytes to be encoded.
	 * @return String object with encoded data.
	 */
	static MAUtil::String base64Encode(const void* data, int length);

	/**
	 * Decode Base64 encoded data. NOTE: The data buffer returned in
	 * the output parameter must be deallocated using free().
	 * @param input Pointer to zero-terminated string with encoded data.
	 * @param output Pointer to pointer that will point to the decoded data.
	 * @param input Pointer int that will contain the length of the decoded data.
	 * @return 1 on success, 0 on error.
	 */
	static int base64Decode(const char* input, void** output, int* outputLength);
};

} // namespace

#endif

/*! @} */
