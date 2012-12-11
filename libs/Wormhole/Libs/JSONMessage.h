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
		 * @return The string value of a field in the JSON data.
		 * Return empty string if the field does not exist.
		 */
		virtual MAUtil::String getArgsField(const MAUtil::String& fieldName);

		/**
		 * @return The integer value of a field in the JSON data.
		 * Return 0 if the field does not exist.
		 */
		virtual int getArgsFieldInt(const MAUtil::String& fieldName);

		/**
		 * Get the options parameters "create" and "exclusive"
		 * from the JSON tree.
		 * @return true on success, false on error.
		 */
		virtual bool getJSONParamsOptionsCreateExclusive(
			bool& create,
			bool& exclusive);

		/**
		 * Get the parent fullPath of a directory entry from the JSON tree.
		 * @return true on success, false on error.
		 */
		virtual bool getJSONParamParentFullPath(MAUtil::String& destinationPath);
	};
} // namespace

#endif

/*! @} */
