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

using namespace MAUtil;

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
MAUtil::String PhoneGapMessage::getJSONField(const MAUtil::String& fieldName)
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
int PhoneGapMessage::getJSONFieldInt(const MAUtil::String& fieldName)
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
