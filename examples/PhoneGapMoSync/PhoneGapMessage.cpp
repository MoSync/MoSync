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
 * @file PhoneGapMessage.cpp
 * @author Mikael Kindborg
 *
 * Class for parsing messages from a WebView.
 */

#include "PhoneGapMessage.h"
#include <mastring.h>

using namespace MAUtil;

static void CharToHex(unsigned char c, char* hexBuf)
{
	const char* hexchar = "0123456789ABCDEF";
	hexBuf[0] = hexchar[c >> 4];
	hexBuf[1] = hexchar[c & 0x0F];
}

/**
 * Make a JSON stringified string.
 */
String PhoneGapMessage::JSONStringify(const char* str)
{
	String jsonString = "\"";
	int length = strlen(str);
	char hexBuf[8];
	hexBuf[0] = '\\';
	hexBuf[1] = '\\';
	hexBuf[2] = 'u';
	hexBuf[3] = '0';
	hexBuf[4] = '0';
	hexBuf[7] = 0;
	char strBuf[2];
	strBuf[1] = 0;

	for (int i = 0; i < length; ++i)
	{
		strBuf[0] = str[i];
		char* escaped = strBuf;
		switch (str[i])
		{
			case '\r': escaped = "\\\\r"; break;
			case '\n': escaped = "\\\\n"; break;
			case '\\': escaped = "\\\\\\\\"; break;
			case '/': escaped = "\\\\/"; break;
			case '"': escaped = "\\\\\""; break;
			case '\f': escaped = "\\\\f"; break;
			case '\b': escaped = "\\\\b"; break;
			case '\t': escaped = "\\\\t"; break;
			default:
				if ((unsigned char) str[i] < 32)
				{
					CharToHex(str[i], hexBuf + 5);
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
 * Based on: http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
 */
String PhoneGapMessage::base64Encode(const char* input)
{
	const char* encodeLookup =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const char padCharacter = '=';

	String encodedString;
	int inputLength = strlen(input);
	int padCount = inputLength % 3;
	encodedString.reserve(((inputLength / 3) + (padCount > 0)) * 4);

	int i;
	unsigned long temp;

	for (i = 0; i < inputLength - 2; ++i)
	{
		// Convert to big endian.
		temp  = ((unsigned char)input[i])   << 16;
		temp += ((unsigned char)input[++i]) << 8;
		temp += ((unsigned char)input[++i]);

		encodedString += encodeLookup[(temp & 0x00FC0000) >> 18];
		encodedString += encodeLookup[(temp & 0x0003F000) >> 12];
		encodedString += encodeLookup[(temp & 0x00000FC0) >> 6 ];
		encodedString += encodeLookup[(temp & 0x0000003F)      ];
	}

	switch (padCount)
	{
		case 1:
			// Convert to big endian.
			temp  = ((unsigned char)input[i]) << 16;
			encodedString += encodeLookup[(temp & 0x00FC0000) >> 18];
			encodedString += encodeLookup[(temp & 0x0003F000) >> 12];
			encodedString += padCharacter;
			encodedString += padCharacter;
			break;
		case 2:
			// Convert to big endian.
			temp  = ((unsigned char)input[i]) << 16;
			temp += ((unsigned char)input[++i]) << 8;
			encodedString += encodeLookup[(temp & 0x00FC0000) >> 18];
			encodedString += encodeLookup[(temp & 0x0003F000) >> 12];
			encodedString += encodeLookup[(temp & 0x00000FC0) >> 6 ];
			encodedString += padCharacter;
			break;
	}

	return encodedString;
}

/**
 * Constructor.
 */
PhoneGapMessage::PhoneGapMessage(NativeUI::WebView* webView, MAHandle dataHandle) :
	Wormhole::WebViewMessage(webView, dataHandle),
	mJSONRoot(NULL)
{
	if (this->hasParam("args"))
	{
		String args = this->getParam("args");
		// Is this JSON?
		if (MAUtil::String::npos != args.find("{"))
		{
			mJSONRoot = YAJLDom::parse(
				(const unsigned char*)args.c_str(),
				args.size());
		}
	}
}

/**
 * Destructor.
 */
PhoneGapMessage::~PhoneGapMessage()
{
	if (NULL != mJSONRoot)
	{
		// The root must not be NULL or Value::NUL.
		if (NULL != mJSONRoot && YAJLDom::Value::NUL != mJSONRoot->getType())
		{
			// Delete the JSON tree.
			YAJLDom::deleteValue(mJSONRoot);
			mJSONRoot = NULL;
		}
	}
}

/**
 * @return The JSON root node.
 */
MAUtil::YAJLDom::Value* PhoneGapMessage::getJSONRoot()
{
	return mJSONRoot;
}

/**
 * @return The string value of a field at the top-level
 * of the JSON tree. Return empty string if the field
 * does not exist.
 */
MAUtil::String PhoneGapMessage::getArgsField(const MAUtil::String& fieldName)
{
	if (NULL != mJSONRoot)
	{
		YAJLDom::Value* value = mJSONRoot->getValueForKey(fieldName.c_str());
		if (NULL != value && YAJLDom::Value::STRING == value->getType())
		{
			return value->toString();
		}
	}

	return "";
}

/**
 * @return The integer value of a field at the top-level
 * of the JSON tree. Return 0 if the field does not exist.
 */
int PhoneGapMessage::getArgsFieldInt(const MAUtil::String& fieldName)
{
	if (NULL != mJSONRoot)
	{
		YAJLDom::Value* value = mJSONRoot->getValueForKey(fieldName.c_str());
		if (NULL != value && YAJLDom::Value::NUMBER == value->getType())
		{
			return value->toInt();
		}
	}

	return 0;
}

/**
 * Get the options parameters "create" and "exclusive"
 * from the JSON tree.
 * @return true on success, false on error.
 */
bool PhoneGapMessage::getJSONParamsOptionsCreateExclusive(
	bool& create,
	bool& exclusive)
{
	// Default values.
	create = false;
	exclusive = false;

	YAJLDom::Value* argsNode = getJSONRoot();
	if (NULL == argsNode || YAJLDom::Value::NUL == argsNode->getType())
	{
		return false;
	}

	YAJLDom::Value* optionsNode = argsNode->getValueForKey("options");
	if (NULL != optionsNode && YAJLDom::Value::NUL != optionsNode->getType())
	{
		YAJLDom::Value* createNode = optionsNode->getValueForKey("create");
		if (NULL != optionsNode && YAJLDom::Value::NUL != optionsNode->getType())
		{
			create = createNode->toBoolean();
		}
		YAJLDom::Value* exclusiveNode = optionsNode->getValueForKey("exclusive");
		if (NULL != exclusiveNode && YAJLDom::Value::NUL != exclusiveNode->getType())
		{
			exclusive = exclusiveNode->toBoolean();
		}

		return true;
	}

	return false;
}

/**
 * Get the parent fullPath of a directory entry from the JSON tree.
 * @return true on success, false on error.
 */
bool PhoneGapMessage::getJSONParamParentFullPath(String& destinationPath)
{
	// Get the root node for the message parameters.
	YAJLDom::Value* argsNode = getJSONRoot();
	if (NULL == argsNode || YAJLDom::Value::NUL == argsNode->getType())
	{
		return false;
	}

	// Get the node for the parent directory.
	YAJLDom::Value* parentNode = argsNode->getValueForKey("parent");
	if (NULL != parentNode && YAJLDom::Value::MAP == parentNode->getType())
	{
		YAJLDom::Value* pathNode = parentNode->getValueForKey("fullPath");
		if (NULL != pathNode && YAJLDom::Value::NUL != pathNode->getType())
		{
			destinationPath = pathNode->toString();
			return true;
		}
	}

	return false;
}
