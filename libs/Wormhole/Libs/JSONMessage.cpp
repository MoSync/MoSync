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
 * @file JSONMessage.cpp
 * @author Mikael Kindborg
 *
 * Class for parsing JSON messages from a WebView.
 */

#include "JSONMessage.h"
#include <mastring.h>

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	JSONMessage::JSONMessage(
		MAWidgetHandle webViewHandle,
		MAHandle dataHandle) :
		Wormhole::MessageStreamJSON(webViewHandle, dataHandle)
	{
	}

	/**
	 * Constructor.
	 * @deprecated
	 */
	JSONMessage::JSONMessage(
		NativeUI::WebView* webView,
		MAHandle dataHandle) :
		Wormhole::MessageStreamJSON(webView, dataHandle)
	{
	}

	/**
	 * Destructor.
	 */
	JSONMessage::~JSONMessage()
	{
	}

	MAUtil::String JSONMessage::getArgsField(const MAUtil::String& fieldName)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::MAP == argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueForKey(fieldName.c_str());
			if (NULL != value && YAJLDom::Value::STRING == value->getType())
			{
				return value->toString();
			}
		}
		return "";
	}

	MAUtil::String JSONMessage::getArgsField(int index)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::ARRAY == argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueByIndex(index);
			if (NULL != value && YAJLDom::Value::STRING == value->getType())
			{
				return value->toString();
			}
		}
		return "";
	}

	int JSONMessage::getArgsFieldInt(const MAUtil::String& fieldName)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::MAP == argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueForKey(fieldName.c_str());
			if (NULL != value && YAJLDom::Value::NUMBER == value->getType())
			{
				return value->toInt();
			}
		}
		return 0;
	}

	int JSONMessage::getArgsFieldInt(int index)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::ARRAY == argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueByIndex(index);
			if (NULL != value && YAJLDom::Value::NUMBER == value->getType())
			{
				return value->toInt();
			}
		}
		return 0;
	}

} // namespace
