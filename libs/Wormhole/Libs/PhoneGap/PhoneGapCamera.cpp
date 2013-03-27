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

#include <conprint.h>
#include <MAUtil/String.h>
#include <mastring.h>
#include <maxtoa.h>
#include "PhoneGapMessageHandler.h"
#include "PhoneGapCamera.h"

#define MYLOG(s) maWriteLog(s, strlen(s))

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	PhoneGapCamera::PhoneGapCamera(PhoneGapMessageHandler* messageHandler)
		: mMessageHandler(messageHandler)
	{
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);
	}

	/**
	 * Destructor.
	 */
	PhoneGapCamera::~PhoneGapCamera()
	{
	}

	/**
	 * Implementation of capture API:s exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	void PhoneGapCamera::handleMessage(JSONMessage& message)
	{
		if (message.getParam("action") == "takePicture")
		{
			MYLOG("PhoneGapCamera::handleMessage takePicture");

			mCaptureCallBack = message.getParam("PhoneGapCallBackId");

			int quality = message.getArgsFieldInt(0);
			destinationType = (DestinationType)message.getArgsFieldInt(1);
			PictureSourceType sourceType = (PictureSourceType)message.getArgsFieldInt(2);
			int targetWidth = message.getArgsFieldInt(3);
			int targetHeight = message.getArgsFieldInt(4);
			EncodingType encodingType = (EncodingType)message.getArgsFieldInt(5);
			MediaType mediaType = (MediaType)message.getArgsFieldInt(6);
			bool allowEdit = message.getArgsFieldBool(7);
			bool correctOrientation = message.getArgsFieldBool(8);
			bool savePhotoToAlbum = message.getArgsFieldBool(9);

			//MAUtil::String popoverOptions = message.getArgsField(10);

			lprintfln("Camera options: %d %d %d %d %d %d %d %s %s %s", quality, destinationType, sourceType,
				targetWidth, targetHeight, encodingType, mediaType,
				(allowEdit ? "true" : "false"),
				(correctOrientation ? "true" : "false"),
				(savePhotoToAlbum ? "true" : "false"));

			switch (sourceType)
			{
				case PHOTOLIBRARY:
					if (destinationType == DATA_URL)
					{
						// #EVENT_TYPE_IMAGE_PICKER
						maImagePickerOpenWithEventReturnType(MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA);
					}
					else
					{
						mMessageHandler->callError(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_ERROR,
							"{\"code\":\"DESTINATION_TYPE_FILE_URI_NOT_AVAILABLE\"}",
							false);
					}
					break;
				case CAMERA:
					startCamera(quality, targetWidth, targetHeight,
						encodingType, correctOrientation, savePhotoToAlbum);
					break;
				case SAVEDPHOTOALBUM:
					mMessageHandler->callError(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_ERROR,
							"{\"code\":\"SOURCE_TYPE_SAVEDPHOTOALBUM_NOT_AVAILABLE\"}",
							false);
					break;
				default: // not a valid value
					mMessageHandler->callError(
						mCaptureCallBack,
						PHONEGAP_CALLBACK_STATUS_ERROR,
						"{\"code\":\"INVALID_SOURCE_TYPE\"}",
						false);
			}
		}
	}

	/**
	 * Event handler for camera/image picker events
	 * @param event the event struct.
	 */
	void PhoneGapCamera::customEvent(const MAEvent &event)
	{
		if ( event.type == EVENT_TYPE_IMAGE_PICKER)
		{
			if ( event.imagePickerState == 1 )
			{
				if (destinationType == DATA_URL)
				{
					int dataSize = maGetDataSize(event.imagePickerItem);

					lprintfln("Data size: %d", dataSize);

					char* imageData = new char[dataSize];
					memset(imageData, 0, dataSize);
					maReadData(event.imagePickerItem, imageData, 0, dataSize);

					lprintfln("Data: %s", imageData);

				/*	char messageBuffer[dataSize+2];
					sprintf(
						messageBuffer,
						"\"%s\"",
						imageData);

					mMessageHandler->callSuccess(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_OK,
							messageBuffer,
							false); */

					delete imageData;

					//sprintf(dataBuffer, "%s", (char*)imageData);

					if (imageData != NULL)
					{
					/*	mMessageHandler->callSuccess(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_OK,
							(char*)imageData,
							false); */
					}
					else
					{
						mMessageHandler->callError(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_ERROR,
							"{\"code\":\"INVALID_DESTINATION_TYPE\"}",
							false);
					}
				}
				else
				{
					mMessageHandler->callError(
						mCaptureCallBack,
						PHONEGAP_CALLBACK_STATUS_ERROR,
						"{\"code\":\"INVALID_DESTINATION_TYPE\"}",
						false);
				}
			}
			else
			{
				mMessageHandler->callError(
					mCaptureCallBack,
					PHONEGAP_CALLBACK_STATUS_ERROR,
					"{\"code\":\"USER_CANCELED_IMAGE_SELECTION\"}",
					false);
			}
		}
	}

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
	 void PhoneGapCamera::startCamera(int quality, int targetWidth, int targetHeight,
					EncodingType encodingType, bool correctOrientation, bool savePhotoToAlbum)
	 {

	 }
} // namespace
