/* Copyright (C) 2013 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/**
 * @file Camera.h
 * @author Mircea Vasiliniuc
 *
 * \brief An instance of Camera allows users to handle camera settings
 * and camera preview layout.
 */
#include "Camera.h"
#include "WidgetUtil.h"

#include <MAUtil/util.h>
#include <MAUtil/CameraEventManager.h>

#include <mavsprintf.h>
#include <mastdlib.h>
#include <madmath.h>

#define CAMERA_BUF_SIZE 256 // size of buffer used for camera string properties.
#define SNAPSHOT_TIMEOUT 3000 // wait time for a snapshot event in milliseconds.

namespace NativeUI
{
	void CameraController::snapshotFailed( const MAHandle& snapshotHandle )
	{
	}


	void CameraController::snapshotSucceeded( const CameraSnapshotData& imageData )
	{
	}


	bool CameraController::previewFrameReady(int *const previewBuffer,
			int previewType,
			MARect previewRect)
	{
		return false;
	};


	Camera::Camera():
		Widget(MAW_CAMERA_PREVIEW)
		,mController(NULL)
		,mCameraPreviewBuffer(NULL)
		,mPreviewRect(MARect())
		,mCurrentCameraIndex(0)
		,mIsCameraStarted(false)
	{
		// Set some layout common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();

		MAUtil::CameraEventManager::getInstance()->addCameraListener(this);
	}


	Camera::~Camera()
	{
		MAUtil::CameraEventManager::getInstance()->removeCameraListener(this);
	}


	void Camera::startPreview()
	{
		int returnCode = maCameraSetPreview(this->getWidgetHandle());

		if ( returnCode >= 0 )
		{
			returnCode = maCameraStart();

			if ( returnCode >= 0 )
			{
				mIsCameraStarted = true;
			}
		}
	}


	void Camera::stopPreview()
	{
		int returnCode = maCameraStop();

		if ( returnCode >= 0 )
		{
			mIsCameraStarted = false;
		}
	}


	bool Camera::isCameraStarted() const
	{
		return mIsCameraStarted;
	}


	const MAUtil::Vector<MAExtent>& Camera::getAvailableSnapshotSizes()
	{
		if ( 0 == mSnapshotSizes.size() )
		{
			int returnValue = maCameraFormatNumber();

			if ( returnValue  > 0 )
			{
				MA_CAMERA_FORMAT cameraformat;
				for ( int index = 0; index < returnValue; index++ )
				{
					maCameraFormat(index, &cameraformat);

					// convert from MA_CAMERA_FORMAT to EXTENT so its
					// symmetric with getPreviewSize()
					int size = EXTENT((int) cameraformat.width, (int)cameraformat.height);
					mSnapshotSizes.add(size);
				}
			}
		}
		return mSnapshotSizes;
	}


	void Camera::setCurrentCameraByIndex( const unsigned int cameraNumber )
	{
		// Even if the camera number is incorrect the camera is stopped.
		maCameraStop();

		int resultCode =  maCameraSelect(cameraNumber);

		if ( resultCode > 0 )
		{
			mCurrentCameraIndex = cameraNumber;

			// We need to restart the preview if the camera was started.
			if ( mIsCameraStarted )
			{
				startPreview();
			}
			mSnapshotSizes.clear();
		}
	}


	unsigned int Camera::getCurrentCameraIndex() const
	{
		return mCurrentCameraIndex;
	}


	unsigned int Camera::getNumberOfAvailableCameras() const
	{
		return maCameraNumber();
	}


	inline void Camera::failSnapshot( const MAHandle& snapshotHandle )
	{
		if (mController)
		{
			mController->snapshotFailed(snapshotHandle);
		}
	}


	inline void Camera::succeedSnapshot( CameraSnapshotData* imageData )
	{
		if (mController)
		{
			mController->snapshotSucceeded( *imageData );
		}
	}


	inline void Camera::restoreCamera()
	{
		// We need to restore the camera only for the Android.
		if ( MAUtil::OS_ANDROID ==
			MAUtil::Environment::getEnvironment().getCurrentPlatform() )
		{
			stopPreview();
			startPreview();
		}
	}


	void Camera::takeSnapshot( MAHandle dataPlaceholder, int sizeIndex )
	{
		// If input is not valid or if triggering the snapshot returns an error it
		// immediately sends a fail snapshot event.
		if ( !isSnapshotInputValid( dataPlaceholder, sizeIndex )
			|| ( MA_CAMERA_RES_OK != maCameraSnapshotAsync( dataPlaceholder, sizeIndex) ) )
		{
			failSnapshot(dataPlaceholder);
		}
	}


	bool Camera::isSnapshotInputValid( MAHandle dataPlaceholder, int sizeIndex  )
	{
		bool isValid = true;

		if ( dataPlaceholder <= 0 )
		{
			// If the data place-holder provided is invalid we deny the request for a snapshot.
			isValid = false;
		}
		else if ( sizeIndex > mSnapshotSizes.size() ||
				(( 0 > sizeIndex) && (MA_CAMERA_SNAPSHOT_MAX_SIZE != sizeIndex)) )
		{
			// If the size index provided is not available we deny the request for a snapshot.
			isValid = false;
		}
		return isValid;
	}


	void Camera::handleCameraEvent( const MAEvent& event )
	{
		switch (event.type)
		{
			case EVENT_TYPE_CAMERA_SNAPSHOT:
				manageSnapshotEvent( event );
				break;
			case EVENT_TYPE_CAMERA_PREVIEW:
				managePreviewEvent( event );
				break;
			default:
				// future events
				break;
		}
	}


	void Camera::manageSnapshotEvent( const MAEvent& snapshotEvent )
	{
		restoreCamera();

		// If we have a valid snapshot we pass the handling.
		if ( snapshotEvent.snapshotReturnCode == MA_CAMERA_RES_OK )
		{
			CameraSnapshotData* imageData = CameraSnapshotData::makeFromEvent( snapshotEvent );
			succeedSnapshot( imageData );
			delete imageData;
		}
		else
		{
			failSnapshot(snapshotEvent.snapshotImageDataHandle);
		}
	}


	void Camera::managePreviewEvent( const MAEvent& previewEvent )
	{
		// If no controller is set there is no reason to proceed.
		if ( mController )
		{
			// Check if we need another frame from the camera.
			bool shouldRequestAnotherFrame = mController->previewFrameReady(
				mCameraPreviewBuffer, previewEvent.status, mPreviewRect);

			if ( shouldRequestAnotherFrame )
			{
				maCameraPreviewEventConsumed();
			}
		}
	}


	void Camera::startRecord()
	{
		maCameraRecord(true);
	}


	void Camera::stopRecord()
	{
		maCameraRecord(false);
	}


	unsigned int Camera::getPreviewSize() const
	{
		return maCameraPreviewSize();
	}


	bool Camera::setFlashMode( const CAMERA_FLASH_MODE flashMode )
	{
		MAUtil::String stringValue = "";

		switch (flashMode)
		{
			case FLASH_AUTO:
				stringValue = MA_CAMERA_FLASH_AUTO;
				break;
			case FLASH_ON:
				stringValue = MA_CAMERA_FLASH_ON;
				break;
			case FLASH_OFF:
				stringValue = MA_CAMERA_FLASH_OFF;
				break;
			case FLASH_TORCH:
				stringValue = MA_CAMERA_FLASH_TORCH;
				break;
			default:
				break;
		}

		int returnCode = this->setCameraPropertyString(MA_CAMERA_FLASH_MODE,
			stringValue);
		return (returnCode > 0) ? true : false;
	}


	CAMERA_FLASH_MODE Camera::getFlashMode() const
	{
		MAUtil::String resultString = this->getCameraPropertyString(MA_CAMERA_FLASH_MODE);

		CAMERA_FLASH_MODE returnValue = FLASH_UNDEFINED;

		if ( resultString == MA_CAMERA_FLASH_AUTO )
		{
			returnValue = FLASH_AUTO;
		}
		else if ( resultString == MA_CAMERA_FLASH_ON )
		{
			returnValue = FLASH_ON;
		}
		else if ( resultString == MA_CAMERA_FLASH_OFF )
		{
			returnValue = FLASH_OFF;
		}
		else if ( resultString == MA_CAMERA_FLASH_TORCH )
		{
			returnValue = FLASH_TORCH;
		}

		return returnValue;
	}


	bool Camera::setFocusMode( const CAMERA_FOCUS_MODE focusMode )
	{
		MAUtil::String stringValue = "";

		switch (focusMode)
		{
			case FOCUS_AUTO:
				stringValue = MA_CAMERA_FOCUS_AUTO;
				break;
			case FOCUS_INFINITY:
				stringValue = MA_CAMERA_FOCUS_INFINITY;
				break;
			case FOCUS_MACRO:
				stringValue = MA_CAMERA_FOCUS_MACRO;
				break;
			case FOCUS_FIXED:
				stringValue = MA_CAMERA_FOCUS_FIXED;
				break;
			default:
				break;
		}

		int returnCode = this->setCameraPropertyString(MA_CAMERA_FOCUS_MODE,
			stringValue);
		return (returnCode > 0) ? true : false;
	}


	CAMERA_FOCUS_MODE Camera::getFocusMode() const
	{
		MAUtil::String resultString = this->getCameraPropertyString(MA_CAMERA_FOCUS_MODE);

		CAMERA_FOCUS_MODE returnValue = FOCUS_UNDEFINED;

		if ( resultString == MA_CAMERA_FOCUS_AUTO )
		{
			returnValue = FOCUS_AUTO;
		}
		else if ( resultString == MA_CAMERA_FOCUS_INFINITY )
		{
			returnValue = FOCUS_INFINITY;
		}
		else if ( resultString == MA_CAMERA_FOCUS_MACRO )
		{
			returnValue = FOCUS_MACRO;
		}
		else if ( resultString == MA_CAMERA_FOCUS_FIXED )
		{
			returnValue = FOCUS_FIXED;
		}

		return returnValue;
	}


	bool Camera::setSnapshotFormat( const CAMERA_SNAPSHOT_FORMAT snapshotFormat )
	{
		MAUtil::String stringValue = "";

		switch (snapshotFormat)
		{
			case SNAPSHOT_RAW:
				stringValue = MA_CAMERA_IMAGE_RAW;
				break;
			case SNAPSHOT_JPEG:
				stringValue = MA_CAMERA_IMAGE_JPEG;
				break;
			default:
				break;
		}

		int returnCode = this->setCameraPropertyString(MA_CAMERA_IMAGE_FORMAT,
			stringValue);
		return (returnCode > 0) ? true : false;
	}


	CAMERA_SNAPSHOT_FORMAT Camera::getSnapshotFormat() const
	{
		MAUtil::String resultString = this->getCameraPropertyString(MA_CAMERA_IMAGE_FORMAT);

		CAMERA_SNAPSHOT_FORMAT returnValue = SNAPSHOT_UNKNOWN;

		if ( resultString == MA_CAMERA_IMAGE_RAW )
		{
			returnValue = SNAPSHOT_RAW;
		}
		else if ( resultString == MA_CAMERA_IMAGE_JPEG )
		{
			returnValue = SNAPSHOT_JPEG;
		}

		return returnValue;
	}


	bool Camera::setZoomLevel( const unsigned int zoomLevel )
	{
		int returnCode = this->setCameraPropertyInt(MA_CAMERA_ZOOM, zoomLevel);
		return (returnCode > 0) ? true : false;
	}


	unsigned int Camera::getZoomLevel() const
	{
		return this->getCameraPropertyInt(MA_CAMERA_ZOOM);
	}


	unsigned int Camera::getMaxZoomLevel() const
	{
		return this->getCameraPropertyInt(MA_CAMERA_MAX_ZOOM);
	}


	bool Camera::isZoomSupported() const
	{
		MAUtil::String value = MAUtil::lowerString(
			this->getCameraPropertyString(MA_CAMERA_ZOOM_SUPPORTED));

		if ( 0 != strcmp(value.c_str(), "true") )
		{
			return false;
		}
		return true;
	}


	bool Camera::isFlashSupported() const
	{
		MAUtil::String value = MAUtil::lowerString(
			this->getCameraPropertyString(MA_CAMERA_FLASH_SUPPORTED));

		if ( 0 != strcmp(value.c_str(), "true") )
		{
			return false;
		}
		return true;
	}


	void Camera::startRetrievingCameraPreviewData(
		const int previewType, const MARect previewRect )
	{
		if ( mIsCameraStarted )
		{
			stopRetrievingCameraPreviewData();

			// Create buffer for the preview data.
			int size = previewRect.width * previewRect.height;
			mCameraPreviewBuffer = new int[size];

			int resultCode = maCameraPreviewEventEnable(previewType, mCameraPreviewBuffer,
				&previewRect);

			if ( resultCode >= 0 )
			{
				mPreviewRect = previewRect;
			}
			else
			{
				// Delete buffer if enabling of the preview data failed.
				delete [] mCameraPreviewBuffer;
				mCameraPreviewBuffer = NULL;
			}
		}
	}


	void Camera::stopRetrievingCameraPreviewData()
	{
		if ( mIsCameraStarted )
		{
			maCameraPreviewEventDisable();

			if ( NULL != mCameraPreviewBuffer )
			{
				delete [] mCameraPreviewBuffer;
				mCameraPreviewBuffer = NULL;
			}
		}
	}


	void Camera::setCameraController( CameraController* controller )
	{
		if ( NULL != controller )
		{
			mController = controller;
		}
	}


	int Camera::setCameraPropertyInt(
		const MAUtil::String& cameraProperty,
		const int value)
	{
		char valueBuffer[CAMERA_BUF_SIZE];
		sprintf(valueBuffer, "%d", value);

		int returnCode = maCameraSetProperty( cameraProperty.c_str(), valueBuffer );
		return returnCode;
	}


	int Camera::setCameraPropertyString(
		const MAUtil::String& cameraProperty,
		const MAUtil::String& value)
	{
		return maCameraSetProperty( cameraProperty.c_str(), value.c_str() );
	}


	int Camera::getCameraPropertyInt(
		const MAUtil::String& cameraProperty) const
	{
		char valueBuffer[CAMERA_BUF_SIZE];
		int returnCode = maCameraGetProperty( cameraProperty.c_str(),
			valueBuffer, CAMERA_BUF_SIZE );

		if ( returnCode > 0 )
		{
			return atoi(valueBuffer);
		}
		return returnCode;
	}


	MAUtil::String Camera::getCameraPropertyString(
		const MAUtil::String& cameraProperty ) const
	{
		char valueBuffer[CAMERA_BUF_SIZE];
		int returnCode = maCameraGetProperty(
			cameraProperty.c_str(),
			valueBuffer,
			CAMERA_BUF_SIZE );

		if ( returnCode < 0 )
		{
			sprintf( valueBuffer, "%d", returnCode );
		}
		return valueBuffer;
	}

} // namespace NativeUI
