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
 * @file Camera.h
 * @author Mircea Vasiliniuc
 *
 * \brief An instance of Camera allows users to handle camera settings
 * and camera preview layout.
 */
#include "Camera.h"
#include "WidgetUtil.h"

#include <MAUtil/util.h>

#include <mavsprintf.h>
#include <mastdlib.h>
#include <madmath.h>

#define STRING_PROPERTY_SIZE 256 // size of buffer used for camera string properties.
#define SNAPSHOT_TIMEOUT 3000 // wait time for a snapshot event in milliseconds.

#define INVALID_VALUE ""

namespace NativeUI
{
	CameraSnapshotData* CameraSnapshotData::makeFromEvent( const MAEvent& event )
	{
		CameraSnapshotData* data = new CameraSnapshotData();
		data->dataHandle = event.snapshotImageDataHandle;
		data->imageSize = event.snapshotSize;
		data->resultCode = event.snapshotReturnCode;

		// Convert image representation to CAMERA_SNAPSHOT_FORMAT
		switch (event.snapshotImageDataRepresentation)
		{
		case MA_IMAGE_REPRESENTATION_RAW:
			data->format = SNAPSHOT_RAW;
			break;
		case MA_IMAGE_REPRESENTATION_JPEG:
			data->format = SNAPSHOT_JPEG;
			break;
		default:
			data->format = SNAPSHOT_UNKNOWN;
			break;
		}

		return data;
	}


	Camera::Camera():
		Widget(MAW_CAMERA_PREVIEW),
		mPreviewDataListenerRef(NULL),
		mCameraPreviewBuffer(NULL),
		mPreviewRect(MARect()),
		mCurrentCameraIndex(0),
		mIsCameraStarted(false)
	{
		// Set some layout common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();

		fillSnapshotSizesArray();

		MAUtil::Environment::getEnvironment().addCameraListener(this);
	}


	Camera::~Camera()
	{
		mSnapshotListeners.clear();

		MAUtil::Environment::getEnvironment().removeCameraListener(this);
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
			stopRetrievingCameraPreviewData();
			mIsCameraStarted = false;
		}
	}


	bool Camera::isCameraStarted() const
	{
		return mIsCameraStarted;
	}


	const MAUtil::Vector<MAExtent>& Camera::getAvailableSnapshotSizes()
	{
		return mSnapshotSizes;
	}


	void Camera::fillSnapshotSizesArray()
	{
		mSnapshotSizes.clear();
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


	void Camera::setCurrentCameraByIndex( const unsigned int cameraNumber )
	{
		maCameraStop();

		int resultCode =  maCameraSelect(cameraNumber);

		if ( resultCode > 0 )
		{
			mCurrentCameraIndex = cameraNumber;

			// When the camera is switched while is active we need to restart
			// the preview.
			if ( mIsCameraStarted )
			{
				startPreview();
			}
			fillSnapshotSizesArray();
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


	inline void Camera::failSnapshot( const MAHandle& snapshotHandle,
		int errorCode )
	{
		CameraSnapshotData* imageData = new CameraSnapshotData();
		imageData->dataHandle = snapshotHandle;
		imageData->format = SNAPSHOT_UNKNOWN;
		imageData->imageSize = 0;
		imageData->resultCode = errorCode;

		for ( int i = 0; i < mSnapshotListeners.size(); i++)
		{
			mSnapshotListeners[i]->snapshotFinished(*imageData);
		}
		delete imageData;
	}


	inline void Camera::succeedSnapshot( CameraSnapshotData* imageData )
	{
		for ( int i = 0; i < mSnapshotListeners.size(); i++)
		{
			mSnapshotListeners[i]->snapshotFinished(*imageData);
		}
	}

	inline void Camera::failRetrievingPreviewData( int returnCode,
		const int previewType,
		const MARect previewRect )
	{
		if ( mPreviewDataListenerRef )
		{
			CameraPreviewData* previewData = new CameraPreviewData();

			previewData->frameBuffer = NULL;
			previewData->type = previewType;
			previewData->rect = previewRect;
			previewData->resultCode = returnCode;

			mPreviewDataListenerRef->retrievingPreviewFrameFinished(*previewData);

			delete previewData;
		}
	}


	inline void Camera::restartCamera()
	{
		// On Android preview needs to be restarted after a snapshot in order to
		// work. As a future improvement we can move the restart in the Android
		// runtime but awareness related to backwards compatibility is needed.
		if ( MAUtil::OS_ANDROID ==
			MAUtil::Environment::getEnvironment().getCurrentPlatform() )
		{
			maCameraStop();
			maCameraStart();
		}
	}


	void Camera::takeSnapshot( MAHandle dataPlaceholder, int sizeIndex )
	{
		if ( !isCameraStarted() )
		{
			failSnapshot(dataPlaceholder, MA_CAMERA_RES_NOT_STARTED);
		}
		else if (isSnapshotHandleValid( dataPlaceholder ) &&
				isSnapshotSizeIndexValid( sizeIndex ) )
		{
			int returnCode = maCameraSnapshotAsync( dataPlaceholder, sizeIndex);

			if ( returnCode != MA_CAMERA_RES_OK )
			{
				failSnapshot(dataPlaceholder, returnCode);
			}
		}
		else
		{
			failSnapshot(dataPlaceholder, MA_CAMERA_RES_VALUE_NOTSUPPORTED);
		}
	}


	bool Camera::isSnapshotHandleValid( MAHandle dataPlaceholder )
	{
		bool isValid = true;

		if ( dataPlaceholder <= 0 )
		{
			isValid = false;
		}
		return isValid;
	}


	bool Camera::isSnapshotSizeIndexValid( int sizeIndex  )
	{
		bool isValid = true;
		if ( sizeIndex > mSnapshotSizes.size() )
		{
			isValid = false;
		}
		else if ( sizeIndex < 0 )
		{
			// MA_CAMERA_SNAPSHOT_MAX_SIZE is defined as -1
			if ( MA_CAMERA_SNAPSHOT_MAX_SIZE != sizeIndex )
			{
				isValid = false;
			}
		}
		return isValid;
	}


	bool Camera::isPreviewTypeValid( int previewType )
	{
		bool isValid;
		switch ( previewType )
		{
			case MA_CAMERA_PREVIEW_AUTO_FOCUS:
			case MA_CAMERA_PREVIEW_FRAME:
				isValid = true;
				break;
			default:
				isValid = false;
				break;
		}
		return isValid;
	}


	bool Camera::isFrameRectValid( MARect frameRect )
	{
		bool isValid = true;
		if ( frameRect.width <= 0 )
		{
			isValid = false;
		}
		else if ( frameRect.height <= 0 )
		{
			isValid = false;
		}
		return isValid;
	}


	void Camera::cameraEvent( const MAEvent& event )
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
		restartCamera();

		if ( snapshotEvent.snapshotReturnCode == MA_CAMERA_RES_OK )
		{
			CameraSnapshotData* imageData =
				CameraSnapshotData::makeFromEvent( snapshotEvent );

			succeedSnapshot( imageData );
			delete imageData;
		}
		else
		{
			failSnapshot(snapshotEvent.snapshotImageDataHandle,
				snapshotEvent.snapshotReturnCode);
		}
	}


	void Camera::managePreviewEvent( const MAEvent& previewEvent )
	{
		if ( mPreviewDataListenerRef )
		{
			CameraPreviewData* previewData = new CameraPreviewData();

			previewData->frameBuffer = mCameraPreviewBuffer;
			previewData->type = previewEvent.status;
			previewData->rect = mPreviewRect;
			previewData->resultCode = MA_CAMERA_RES_OK;

			bool shouldRequestAnotherFrame =
					mPreviewDataListenerRef->retrievingPreviewFrameFinished(*previewData);

			// We need safe-delete macro here when it is available. Also replace other
			// delete operations in this file with the safe-delete
			delete previewData;

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
		MAUtil::String stringValue = INVALID_VALUE;

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
		MAUtil::String resultString =
			this->getCameraPropertyString(MA_CAMERA_FLASH_MODE);

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
		MAUtil::String resultString =
			this->getCameraPropertyString(MA_CAMERA_FOCUS_MODE);

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
		MAUtil::String resultString =
			this->getCameraPropertyString(MA_CAMERA_IMAGE_FORMAT);

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
		if ( !isCameraStarted() )
		{
			failRetrievingPreviewData(MA_CAMERA_RES_NOT_STARTED,
				previewType, previewRect);
		}
		else if (isPreviewTypeValid(previewType) &&
				isFrameRectValid(previewRect))
		{
			stopRetrievingCameraPreviewData();

			// Create buffer for the preview data.
			int size = previewRect.width * previewRect.height;
			mCameraPreviewBuffer = new int[size];

			int resultCode =
				maCameraPreviewEventEnable(previewType, mCameraPreviewBuffer, &previewRect);

			if ( resultCode == MA_CAMERA_RES_OK )
			{
				mPreviewRect = previewRect;
			}
			else
			{
				failRetrievingPreviewData(resultCode, previewType, previewRect);

				// Since this class has the ownership of the mCameraPreviewBuffer
				// it needs to deallocate this memory in case the operation failed.
				delete [] mCameraPreviewBuffer;
				mCameraPreviewBuffer = NULL;
			}
		}
		else
		{
			failRetrievingPreviewData(MA_CAMERA_RES_VALUE_NOTSUPPORTED,
							previewType, previewRect);
		}
	}


	void Camera::stopRetrievingCameraPreviewData()
	{
		if ( isCameraStarted() )
		{
			maCameraPreviewEventDisable();

			if ( mCameraPreviewBuffer )
			{
				delete [] mCameraPreviewBuffer;
				mCameraPreviewBuffer = NULL;
			}
		}
	}


    void Camera::addSnapshotListener(CameraSnapshotListener* listener)
    {
        addListenerToVector(mSnapshotListeners, listener);
    }


    void Camera::removeSnapshotListener(CameraSnapshotListener* listener)
    {
       removeListenerFromVector(mSnapshotListeners, listener);
    }


	void Camera::setCameraPreviewDataListener( CameraPreviewDataListener* listener )
	{
		mPreviewDataListenerRef = listener;
	}


	int Camera::setCameraPropertyInt(
		const MAUtil::String& cameraProperty,
		const int value)
	{
		char valueBuffer[STRING_PROPERTY_SIZE];
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
		char valueBuffer[STRING_PROPERTY_SIZE];
		int returnCode = maCameraGetProperty( cameraProperty.c_str(),
			valueBuffer, STRING_PROPERTY_SIZE );

		if ( returnCode > 0 )
		{
			return atoi(valueBuffer);
		}
		return returnCode;
	}


	MAUtil::String Camera::getCameraPropertyString(
		const MAUtil::String& cameraProperty ) const
	{
		char valueBuffer[STRING_PROPERTY_SIZE];
		int returnCode = maCameraGetProperty(
			cameraProperty.c_str(),
			valueBuffer,
			STRING_PROPERTY_SIZE );

		if ( returnCode < 0 )
		{
			sprintf( valueBuffer, "%d", returnCode );
		}
		return valueBuffer;
	}

} // namespace NativeUI
