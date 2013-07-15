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
#include <mavsprintf.h>
#include "PhoneGapMessageHandler.h"
#include "PhoneGapCamera.h"
#include "PhoneGapCameraUtil.h"

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

			mCameraCallBack = message.getParam("PhoneGapCallBackId");

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
					openImagePicker();
					break;
				case CAMERA:
					startCamera(quality, targetWidth, targetHeight,
						encodingType, correctOrientation, savePhotoToAlbum);
					break;
				case SAVEDPHOTOALBUM:
					openImagePicker();
					break;
				default: // not a valid value
					callError("INVALID_SOURCE_TYPE");
			}
		}
	}

	/**
	 * Opens the image picker with different options, depending on the camera destination tyle (DATA_URL or FILE_URI).
	 */
	void PhoneGapCamera::openImagePicker()
	{
		if (destinationType == DATA_URL)
		{
			// #EVENT_TYPE_IMAGE_PICKER
			maImagePickerOpenWithEventReturnType(MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA);
		}
		else
		{
			callError("DESTINATION_TYPE_FILE_URI_NOT_AVAILABLE");
		}
	}

	/**
	 * Sends an error message to javascript.
	 * @param message The error message.
	 */
	void PhoneGapCamera::callError(const char *message)
	{
		char messageBuffer[1024];

		sprintf(
			messageBuffer,
			"{\"code\":\"%s\"}",
			message);

		mMessageHandler->callError(
				mCameraCallBack,
				PHONEGAP_CALLBACK_STATUS_ERROR,
				messageBuffer,
				false);
	}

	/**
	 * Sends a success message to javascript.
	 * @param data The data to be sent together with the message.
	 */
	void PhoneGapCamera::callSuccess(const char *data)
	{
		mMessageHandler->callSuccess(
					mCameraCallBack,
					PHONEGAP_CALLBACK_STATUS_OK,
					data,
					false);
	}

	/**
	 * Event handler for camera/image picker events
	 * @param event the event struct.
	 */
	void PhoneGapCamera::customEvent(const MAEvent &event)
	{
		if ( event.type == EVENT_TYPE_IMAGE_PICKER)
		{
			handleImagePickerEvent(event);
		}
		else if (event.type == EVENT_TYPE_CAPTURE)
		{
			handleCameraEvent(event);
		}
	}

	void PhoneGapCamera::handleImagePickerEvent(const MAEvent &event)
	{
		if ( event.imagePickerState == 1 )
		{
			if (destinationType == DATA_URL)
			{
				String encodedData = "\"" + readAndBase64EncodeData(event.imagePickerItem) + "\"";

				if (encodedData.size() > 0)
				{
					callSuccess(encodedData.c_str());
				}
				else
				{
					callError("INVALID_IMAGE_DATA");
				}
			}
			else
			{
				// destinationType == FILE_URI
				callError("INVALID_DESTINATION_TYPE");
			}
		}
		else
		{
			callError("USER_CANCELED_IMAGE_SELECTION");
		}
	}

	void PhoneGapCamera::handleCameraEvent(const MAEvent &event)
	{
		char pathBuffer[1024];
		char messageBuffer[1024];
		MACaptureEventData eventData = event.captureData;

		switch (eventData.type)
		{
			case MA_CAPTURE_EVENT_TYPE_VIDEO:
				if (destinationType == FILE_URI)
				{
					// Videos are already stored, we need the filepath
					maCaptureGetVideoPath(
						eventData.handle,
						pathBuffer,
						sizeof(pathBuffer));
					sprintf(
						messageBuffer,
						"[{\"fullPath\":\"%s\",\"name\":\"%s\"}]",
						pathBuffer,
						FileNameFromPath(pathBuffer));
					callSuccess(messageBuffer);
					maCaptureDestroyData(eventData.handle);
				}
				else
				{
					callError("VIDEO_DATA_URL_NOT_AVAILABLE");
				}
				break;
			case MA_CAPTURE_EVENT_TYPE_IMAGE:
			{
				char deviceOS[64];
				String extension;

				maGetSystemProperty(
					"mosync.device.OS",
					deviceOS,
					sizeof(deviceOS));

				// File format is different on different platforms.
				// TODO: What about WP?
				if (strcmp(deviceOS, "iPhone OS") == 0)
				{
					extension = "png";
				}
				else if(strcmp(deviceOS, "Android") == 0)
				{
					extension = "jpg";
				}
				else
				{
					// TODO: What to use as default?
					extension = "image";
				}

				// Images need to be stored. We use maLocalTime to
				// get a unique number for the filename.
				String localPath = (mMessageHandler->getFileUtil())->getAppPath();
				sprintf(
					pathBuffer,
					"%simg%d.%s",
					localPath.c_str(),
					maLocalTime(),
					extension.c_str());
				int result = maCaptureWriteImage(
					eventData.handle,
					pathBuffer,
					sizeof(pathBuffer));

				if (destinationType == FILE_URI)
				{
					sprintf(
						messageBuffer,
						"\"%s\"",
						pathBuffer);

					if (result == MA_CAPTURE_RES_OK)
					{
						callSuccess(messageBuffer);
					}
					else
					{
						callError("CAMERA_INTERNAL_ERR");
					}
				}
				else
				{
					// TODO: encode the data in Base64 and send it to javascript
				}

				// Free capture data.
				maCaptureDestroyData(eventData.handle);

				break;
			}

			case MA_CAPTURE_EVENT_TYPE_CANCEL:
				callError("CAMERA_NO_MEDIA_FILES");
				break;
		} // switch
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
		int result = maCaptureAction(MA_CAPTURE_ACTION_TAKE_PICTURE);

		if(result == MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE)
		{
			//Camera was busy by another app
			callError("CAPTURE_APPLICATION_BUSY");
		}
		if(result == MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED)
		{
			//No camera
			callError("CAPTURE_NOT_SUPPORTED");
		}
	}
} // namespace
