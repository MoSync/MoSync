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
	JSONMessage::JSONMessage(NativeUI::WebView* webView, MAHandle dataHandle) :
		Wormhole::MessageStreamJSON(webView, dataHandle)
	{
	}

	/**
	 * Destructor.
	 */
	JSONMessage::~JSONMessage()
	{
	}

	/**
	 * @return The string value of a field in the JSON data.
	 * Return empty string if the field does not exist.
	 */
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

	/**
	 * @return The integer value of a field in the JSON data.
	 * Return 0 if the field does not exist.
	 */
	int JSONMessage::getArgsFieldInt(const MAUtil::String& fieldName)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::NUL != argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueForKey(fieldName.c_str());
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
	bool JSONMessage::getJSONParamsOptionsCreateExclusive(
		bool& create,
		bool& exclusive)
	{
		// Default values.
		create = false;
		exclusive = false;

		// Get the root node for the message parameters.
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL == argsNode || YAJLDom::Value::MAP != argsNode->getType())
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
	bool JSONMessage::getJSONParamParentFullPath(String& destinationPath)
	{
		// Get the root node for the message parameters.
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL == argsNode || YAJLDom::Value::MAP != argsNode->getType())
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
} // namespace
