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
		 * Handles a image picker event and sends the appropriate message
		 * to javascript (success or error).
		 * @param event The image picker event to be handled.
		 */
		void handleImagePickerEvent(const MAEvent &event);

		/**
		 * Handles a camera event and sends the appropriate message
		 * to javascript (success or error).
		 * @param event The camera event to be handled.
		 */
		void handleCameraEvent(const MAEvent &event);

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

		/**
		 * Opens the image picker with different options, depending on the camera destination tyle (DATA_URL or FILE_URI).
		 */
		void openImagePicker();

		/**
		 * Sends an error message to javascript.
		 * @param message The error message.
		 */
		void callError(const char *message);

		/**
		 * Sends a success message to javascript.
		 * @param data The data to be sent together with the message.
		 */
		void callSuccess(const char *data);
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
		MAUtil::String mCameraCallBack;
	};
} // namespace

#endif
