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


	bool JSONMessage::getArgsFieldBool(const MAUtil::String& fieldName)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::MAP == argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueForKey(fieldName.c_str());
			if (NULL != value && YAJLDom::Value::BOOLEAN == value->getType())
			{
				return value->toBoolean();
			}
		}
		return false;
	}

	bool JSONMessage::getArgsFieldBool(int index)
	{
		YAJLDom::Value* argsNode = getParamNode("args");
		if (NULL != argsNode && YAJLDom::Value::ARRAY == argsNode->getType())
		{
			YAJLDom::Value* value = argsNode->getValueByIndex(index);
			if (NULL != value && YAJLDom::Value::BOOLEAN == value->getType())
			{
				return value->toBoolean();
			}
		}
		return false;
	}

} // namespace
