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
 * @file PhoneGapCapture.cpp
 * @author Iraklis Rossis
 *
 * Implementation of PhoneGap capture calls made from JavaScript.
 */

#include <conprint.h>
#include <MAUtil/String.h>
#include <maxtoa.h>
#include "PhoneGapMessageHandler.h"
#include "PhoneGapCapture.h"

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	PhoneGapCapture::PhoneGapCapture(PhoneGapMessageHandler* messageHandler)
		: mMessageHandler(messageHandler)
	{
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);
	}

	/**
	 * Destructor.
	 */
	PhoneGapCapture::~PhoneGapCapture()
	{
	}

	/**
	 * Implementation of capture API:s exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	void PhoneGapCapture::handleMessage(JSONMessage& message)
	{
		if(message.getParam("action") == "captureVideo")
		{
			int duration = message.getArgsFieldInt("duration");
			if(duration > 0)
			{
				char durationString[16];
				maCaptureSetProperty(MA_CAPTURE_MAX_DURATION, itoa(duration,durationString,10));
			}

			mCaptureCallBack = message.getParam("PhoneGapCallBackId");

			int result = maCaptureAction(MA_CAPTURE_ACTION_RECORD_VIDEO);

			if(result == MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE)
			{
				//Camera was busy by another app
				mMessageHandler->callError(
					mCaptureCallBack,
					PHONEGAP_CALLBACK_STATUS_ERROR,
					"{\"code\":\"CAPTURE_APPLICATION_BUSY\"}",
					false);
			}
			else if(result == MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED)
			{
				//No camera
				mMessageHandler->callError(
					mCaptureCallBack,
					PHONEGAP_CALLBACK_STATUS_ERROR,
					"{\"code\":\"CAPTURE_NOT_SUPPORTED\"}",
					false);
			}
		}
		else if(message.getParam("action") == "captureImage")
		{
			mCaptureCallBack = message.getParam("PhoneGapCallBackId");

			int result = maCaptureAction(MA_CAPTURE_ACTION_TAKE_PICTURE);

			if(result == MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE)
			{
				//Camera was busy by another app
				mMessageHandler->callError(
					mCaptureCallBack,
					PHONEGAP_CALLBACK_STATUS_ERROR,
					"{\"code\":\"CAPTURE_APPLICATION_BUSY\"}",
					false);
			}
			if(result == MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED)
			{
				//No camera
				mMessageHandler->callError(
					mCaptureCallBack,
					PHONEGAP_CALLBACK_STATUS_ERROR,
					"{\"code\":\"CAPTURE_NOT_SUPPORTED\"}",
					false);
			}
		}
		else if(message.getParam("action") == "captureAudio")
		{
			//MoSync does not support audio capture
			mMessageHandler->callError(
				mCaptureCallBack,
				PHONEGAP_CALLBACK_STATUS_ERROR,
				"{\"code\":\"CAPTURE_NOT_SUPPORTED\"}",
				false);
		}
	}

	/**
	 * Event handler for capture events
	 * @param event the event struct.
	 */
	void PhoneGapCapture::customEvent(const MAEvent &event)
	{
		char pathBuffer[1024];
		char messageBuffer[1024];

		if (event.type == EVENT_TYPE_CAPTURE)
		{
			MACaptureEventData eventData = event.captureData;

			switch (eventData.type)
			{
				case MA_CAPTURE_EVENT_TYPE_VIDEO:
					// Videos are already stored, we need the filepath
					maCaptureGetVideoPath(
						eventData.handle,
						pathBuffer,
						sizeof(pathBuffer));
					sprintf(
						messageBuffer,
						"{\"message\":[{\"fullPath\":\"%s\",\"name\":\"%s\"}]}",
						pathBuffer,
						FileNameFromPath(pathBuffer));
					mMessageHandler->callSuccess(
						mCaptureCallBack,
						PHONEGAP_CALLBACK_STATUS_OK,
						messageBuffer,
						false);
					maCaptureDestroyData(eventData.handle);
					break;

				case MA_CAPTURE_EVENT_TYPE_IMAGE:
				{
					char deviceOS[64];
					String extension;
					char localPath[1024];

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
					maGetSystemProperty(
						"mosync.path.local",
						localPath,
						sizeof(localPath));
					sprintf(
						pathBuffer,
						"%simg%d.%s",
						localPath,
						maLocalTime(),
						extension.c_str());
					int result = maCaptureWriteImage(
						eventData.handle,
						pathBuffer,
						sizeof(pathBuffer));
					sprintf(
						messageBuffer,
						"{\"message\":[{\"fullPath\":\"%s\",\"name\":\"%s\"}]}",
						pathBuffer,
						FileNameFromPath(pathBuffer));

					if (result == MA_CAPTURE_RES_OK)
					{
						mMessageHandler->callSuccess(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_OK,
							messageBuffer,
							false);
					}
					else
					{
						mMessageHandler->callError(
							mCaptureCallBack,
							PHONEGAP_CALLBACK_STATUS_ERROR,
							"{\"code\":\"CAPTURE_INTERNAL_ERR\"}",
							false);
					}

					// Free capture data.
					maCaptureDestroyData(eventData.handle);

					break;
				}

				case MA_CAPTURE_EVENT_TYPE_CANCEL:
					mMessageHandler->callError(
						mCaptureCallBack,
						PHONEGAP_CALLBACK_STATUS_ERROR,
						"{\"code\":\"CAPTURE_NO_MEDIA_FILES\"}",
						false);
					break;
			} // switch
		}
	}
} // namespace
