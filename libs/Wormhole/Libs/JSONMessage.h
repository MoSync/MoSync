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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file JSONMessage.h
 * @author Mikael Kindborg
 *
 * Class for parsing JSON messages from a WebView.
 */

#ifndef WORMHOLE_LIBS_JSONMESSAGE_H_
#define WORMHOLE_LIBS_JSONMESSAGE_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <NativeUI/WebView.h>
#include <Wormhole/MessageStreamJSON.h>
#include <yajl/YAJLDom.h>

namespace Wormhole
{
	/**
	 * Class that parses PhoneGap messages.
	 */
	class JSONMessage : public Wormhole::MessageStreamJSON
	{
	public:
		/**
		 * Constructor.
		 */
		JSONMessage(MAWidgetHandle webViewHandle, MAHandle dataHandle);

		/**
		 * Constructor.
		 * @deprecated
		 */
		JSONMessage(NativeUI::WebView* webView, MAHandle dataHandle);

		/**
		 * Destructor.
		 */
		virtual ~JSONMessage();

		/**
		 * Used to retrieve an args field when args is an object.
		 * @param fieldName The key of the field to retrieve.
		 * @return The string value of a field in the JSON data.
		 * Return empty string if the field does not exist.
		 */
		virtual MAUtil::String getArgsField(const MAUtil::String& fieldName);

		/**
		 * Used to retrieve an args element when args is an array.
		 * @param index Zero based index of the array element.
		 * @return The string value of a field in the JSON data.
		 * Return empty string if the field does not exist.
		 */
		virtual MAUtil::String getArgsField(int index);

		/**
		 * Used to retrieve an args field when args is an object.
		 * @param fieldName The key of the field to retrieve.
		 * @return The integer value of a field in the JSON data.
		 * Return 0 if the field does not exist.
		 */
		virtual int getArgsFieldInt(const MAUtil::String& fieldName);

		/**
		 * Used to retrieve an args element when args is an array.
		 * @param index Zero based index of the array element.
		 * @return The integer value of a field in the JSON data.
		 * Return 0 if the field does not exist.
		 */
		virtual int getArgsFieldInt(int index);

		/**
		 * Used to retrieve an args field when args is an object.
		 * @param fieldName The key of the field to retrieve.
		 * @return The boolean value of a field in the JSON data.
		 * Return false if the field does not exist.
		 */
		virtual bool getArgsFieldBool(const MAUtil::String& fieldName);

		/**
		 * Used to retrieve an args element when args is an array.
		 * @param index Zero based index of the array element.
		 * @return The boolean value of a field in the JSON data.
		 * Return false if the field does not exist.
		 */
		virtual bool getArgsFieldBool(int index);

	};
} // namespace

#endif

/*! @} */
