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
 * @file PhoneGapCamera.h
 * @author Mikael Kindborg
 *
 * Implementation of PhoneGap Camera API.
 */

#ifndef WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAMERA_H_
#define WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAMERA_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include "PhoneGapCameraConstants.h"

namespace Wormhole
{
	class JSONMessage;
	class PhoneGapMessageHandler;

	/**
	 * Class that implements PhoneGap Camera API.
	 */
	class PhoneGapCamera: public MAUtil::CustomEventListener
	{
	private:
		/**
		 * Starts the camera and according to the properties sent as parameters
		 * and calls the success/error callback depending on the result.
		 * @param quality Quality of saved image. Range is [0, 100].
		 * @param destinationType The format of the return value. See enum 'DestinationType'.
		 * @param targetWidth Width in pixels to scale image. Must be used with targetHeight. Aspect ratio is maintained.
		 * @param targetHeight Height in pixels to scale image. Must be used with targetWidth. Aspect ratio is maintained.
		 * @param encodingType The encoding of the returned image file. See enum 'EncodingType'.
		 * @param correctOrientation Rotate the image to correct for the orientation of the device during capture.
		 * @param savePhotoToAlbum Rotate the image to correct for the orientation of the device during capture.
		 */
		void startCamera(int quality, int targetWidth, int targetHeight,
					EncodingType encodingType, bool correctOrientation, bool savePhotoToAlbum);
	private:
		/**
		 *	The destination of the image captured. One of the values: DATA_URL, FILE_URI.
		 */
		DestinationType destinationType;
	public:
		/**
		 * Constructor.
		 */
		PhoneGapCamera(PhoneGapMessageHandler* messageHandler);

		/**
		 * Destructor.
		 */
		virtual ~PhoneGapCamera();

		/**
		 * Implementation of Capture API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		virtual void handleMessage(JSONMessage& message);

		/**
		 * Event handler for capture events
		 * @param event the event struct.
		 */
		virtual void customEvent(const MAEvent &event);

	public:
		PhoneGapMessageHandler* mMessageHandler;

		/**
		 * A Pointer to the main webview
		 * Used for communicating with PhoneGap
		 */
		NativeUI::WebView* mWebView;

		/**
		 * Stores the CallbackID of the capture call so the result can be sent
		 * after receiving the media data
		 */
		MAUtil::String mCaptureCallBack;
	};
} // namespace

#endif
