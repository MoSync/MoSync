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
 * @file PhoneGapMessage.h
 * @author Mikael Kindborg
 *
 * Class for parsing messages from a WebView.
 */

#ifndef PHONEGAP_MESSAGE_H_
#define PHONEGAP_MESSAGE_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <NativeUI/WebView.h>
#include <Wormhole/WebViewMessage.h>
#include <yajl/YAJLDom.h>

/**
 * Class that parses PhoneGap messages.
 */
class PhoneGapMessage : public Wormhole::WebViewMessage
{
public:
	/**
	 * Constructor.
	 */
	PhoneGapMessage(NativeUI::WebView* webView, MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~PhoneGapMessage();

	/**
	 * @return The JSON root node.
	 */
	MAUtil::YAJLDom::Value* getJSONRoot();

	/**
	 * @return The string value of a field at the top-level
	 * of the JSON tree. Return empty string if the field
	 * does not exist.
	 */
	MAUtil::String getArgsField(const MAUtil::String& fieldName);

	/**
	 * @return The integer value of a field at the top-level
	 * of the JSON tree. Return 0 if the field does not exist.
	 */
	int getArgsFieldInt(const MAUtil::String& fieldName);

protected:
	MAUtil::YAJLDom::Value* mJSONRoot;
};

#endif
