/*
Copyright (C) 2013 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 FranklStreet, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file Camera.h
 * @author Mircea Vasiliniuc
 *
 * \brief An instance of Camera allows users to handle camera settings
 * and camera preview layout.
 */

#ifndef NATIVEUI_CAMERA_H_
#define NATIVEUI_CAMERA_H_

#include "Widget.h"
#include <MAUtil/Environment.h>

namespace NativeUI
{
	/**
	 * Available flash modes
	 */
	typedef enum {
		FLASH_AUTO = 0,
		FLASH_ON,
		FLASH_OFF,
		FLASH_TORCH,
		FLASH_UNDEFINED
	} CAMERA_FLASH_MODE;

	/**
	 * Available focus modes
	 */
	typedef enum {
		FOCUS_AUTO = 10,
		FOCUS_INFINITY,
		FOCUS_MACRO,
		FOCUS_FIXED,
		FOCUS_UNDEFINED
	} CAMERA_FOCUS_MODE;

	/**
	 * Available image format
	 */
	typedef enum {
		SNAPSHOT_RAW = 100,
		SNAPSHOT_JPEG,
		SNAPSHOT_UNKNOWN
	} CAMERA_SNAPSHOT_FORMAT;

	/**
	 * \brief Container for camera snapshot data
	 */
	typedef struct CameraSnapshotData {
		/**
		 * Handle of the image data
		 */
		MAHandle dataHandle;

		/**
		 * image format
		 */
		CAMERA_SNAPSHOT_FORMAT format;

		/**
		 * Image resolution
		 */
		unsigned int imageSize;

		/**
		 * Result code of the snapshot operation.
		 * See results codes \link #MA_CAMERA_RES_OK here \endlink.
		 */
		int resultCode;

		/**
		 * Method used for creating a camera snapshot data container from
		 * a camera snapshot event.
		 *
		 * \param event reference to a MAEvent.
		 */
		static CameraSnapshotData* makeFromEvent( const MAEvent& event );
	} CameraSnapshotData;

	/**
	 * \brief Container for camera preview data.
	 */
	typedef struct CameraPreviewData {
		/**
		 * Pointer to the array containing frame data.
		 *
		 * Note: Size can be calculated using the CameraPreviewData rect field.
		 * This is the same as the rect used when preview data was requested via
		 * #startRetrievingCameraPreviewData() method of #Camera.
		 */
		int* frameBuffer;

		/**
		 * It holds the preview frame type. It holds
		 * MA_CAMERA_PREVIEW_FRAME when the frame was retrieved as soon as
		 * possible or MA_CAMERA_PREVIEW_AUTO_FOCUS when the frame was
		 * retrieved after focus was made.
		 */
		int type;

		/**
		 * Rectangle of the camera frame inside the camera preview bounds.
		 */
		MARect rect;

		/**
		 * Result code of the operation.
		 * See results codes \link #MA_CAMERA_RES_OK here \endlink.
		 */
		int resultCode;
	} CameraPreviewData;

	/**
	 * Listener for the camera frame data flow.
	 *
	 * Such a listener is needed in order to obtain/manage camera frame flow.
	 *
	 * Note: This can be used only attached to a #Camera instance via
	 * #setCameraPreviewDataListener method.
	 */
	class CameraPreviewDataListener
	{
	public:
		/**
		 * Notifies that the a camera preview frame is ready.
		 *
		 * Note: In order to obtain the preview data this method must be
		 * overridden.
		 *
		 * Note: These events are received only if the preview event firing
		 * was started via #startRetrievingCameraPreviewData method of a
		 * #Camera instance;
		 *
		 * \param previewData struct containg data of the operation of retrieving
		 * a preview frame.
		 *
		 * \return True if the listener needs the next frame, false if
		 * the next frame is not needed and the flow of frames can stop.
		 *
		 * In order to resume the flow of frames another
		 * #startRetrievingCameraPreviewData call is needed.
		 */
		virtual bool retrievingPreviewFrameFinished(
			const CameraPreviewData& previewData) = 0;
	};

	/**
	 * Listener for the camera snapshot events.
	 *
	 * Such a listener is needed in order to obtain camera snapshots.
	 *
	 * Note: This can be used only attached to a #Camera instance via
	 * #addSnapshotListener() method. Remember to deattach listener using
	 * #removeSnapshotListener();
	 */
	class CameraSnapshotListener
	{
	public:
		/**
		 * Notifies that the snapshot operation is finished.
		 *
		 * Note: In order to obtain snapshots from camera via #Camera's
		 * #takeSnapshot method, an implementation of this method is required.
		 *
		 * @param imageData snapshot data that includes the placeholder,
		 * image format, image size or returnCode
		 */
		virtual void snapshotFinished( const CameraSnapshotData& imageData ) = 0;
	};

	/**
	 * \brief Class responsible for camera layout and functionalities.
	 */
	class Camera : public Widget,
				public MAUtil::CameraListener
	{
	public:
		Camera();

		virtual ~Camera();

		/**
		 * Starts the camera preview.
		 */
		virtual void startPreview();

		/**
		 * Stops the camera preview.
		 */
		virtual void stopPreview();

		virtual bool isCameraStarted() const;

		/**
		 * Getter for the available snapshot sizes.
		 * Note: Not available on iOS.
		 *
		 * /return vector containing available snapshot sizes.
		 * The sizes are in MAExtent format.
		 * Use EXTENT_X and EXTENT_Y to obtain width and height.
		 */
		virtual const MAUtil::Vector<MAExtent>& getAvailableSnapshotSizes();

		/**
		 * Selects a camera from the available ones, using the camera index.
		 *
		 * \param cameraIndex index of the camera.
		 * Note: If the camera is running and the index provided is incorrect,
		 * the camera will be stopped.
		 */
		virtual void setCurrentCameraByIndex(const unsigned int cameraIndex);

		/**
		 * Getter for the used camera index.
		 *
		 * \return the index of the camera that is set as current
		 */
		virtual unsigned int getCurrentCameraIndex() const;

		/**
		 * Getter for the number of cameras available on the device.
		 *
		 * \return the number of available cameras on the device.
		 */
		virtual unsigned int getNumberOfAvailableCameras() const;

		/**
		 * Captures a still image of the camera. The output of this operation
		 * is provided via #CameraSnapshotListener's methods #snapshotFailed
		 * and #snapshotSucceeded.
		 *
		 * Note: Any startPreview() or takeSnapshot() calls will be ignored and
		 * will return a error code until the snapshot operation is finished.
		 *
		 * \param dataPlaceholder place-holder where the snapshot data will be
		 * written.
		 *
		 * \param sizeIndex an index of the vector provided by
		 * #getAvailableSnapshotSizes() method. The default value is the
		 * maximum snapshot size.
		 * Note: This argument is ignored on iOS.
		 */
		virtual void takeSnapshot( MAHandle dataPlaceholder,
				int sizeIndex = MA_CAMERA_SNAPSHOT_MAX_SIZE );

		/**
		 * Starts recording video.
		 * Note: Not yet available.
		 */
		virtual void startRecord();

		/**
		 * Stops recording video.
		 * Note: Not yet available.
		 */
		virtual void stopRecord();

		/**
		 * Retrieves the camera frame size. This can be different
		 * from the size of this widget. This is usefull when dealing with
		 * preview data. See #startRetrievingCameraPreviewData()
		 *
		 * Note: This call is available only when camera is running.
		 *
		 * \returns the size MAExtent format.
		 * Use EXTENT_X and EXTENT_Y to obtain width and height.
		 */
		virtual unsigned int getPreviewSize() const;

		/**
		 * Sets the flash mode for the camera.
		 *
		 * \param flashMode a value from the #CAMERA_FLASH_MODE enum.
		 *
		 * \return true if flashMode is supported, false otherwise.
		 */
		virtual bool setFlashMode(const CAMERA_FLASH_MODE flashMode);

		/**
		 * Getter for the used flash mode.
		 *
		 * \return a value from the #CAMERA_FLASH_MODE enum
		 */
		virtual CAMERA_FLASH_MODE getFlashMode() const;

		/**
		 * Sets the focus mode for the camera.
		 *
		 * \param focusMode a value from the #CAMERA_FOCUS_MODE enum
		 *
		 * \return true if focus is supported, false otherwise.
		 */
		virtual bool setFocusMode(const CAMERA_FOCUS_MODE focusMode);

		/**
		 * Getter for the used focus mode.
		 *
		 * \return a value from the #CAMERA_FOCUS_MODE enum
		 */
		virtual CAMERA_FOCUS_MODE getFocusMode() const;

		/**
		 * Sets the format used when retrieving a snapshot via #takeSnapshot
		 * from the camera.
		 *
		 * \param a value from the @CAMERA_SNAPSHOT_FORMAT
		 *
		 * \return true if format is supported, false otherwise.
		 */
		virtual bool setSnapshotFormat(
			const CAMERA_SNAPSHOT_FORMAT snapshotFormat);

		/**
		 * Getter for the used format when taking a snapshot.
		 *
		 * \return a value from the #CAMERA_SNAPSHOT_FORMAT enum
		 */
		virtual CAMERA_SNAPSHOT_FORMAT getSnapshotFormat() const;


		/**
		 * Sets the zoom level of the camera.
		 *
		 * \param zoomLevel integer between 0 and the value
		 * returned by #getMaxZoomLevel() representing zoom level.
		 *
		 * \return true if zoom level is supported, false otherwise.
		 */
		virtual bool setZoomLevel(const unsigned int zoomLevel);

		/**
		 * Getter for the current camera zoom level.
		 *
		 * \return an integer between 0 and the value returned by
		 * #getMaxZoomLevel().
		 */
		virtual unsigned int getZoomLevel() const;

		/**
		 * Getter for maximum supported zoom level on the current camera.
		 *
		 * \return the maximum zoom level available on the current camera.
		 * If zoom is not supported this will return 0.
		 */
		virtual unsigned int getMaxZoomLevel() const;

		/**
		 * Indicates if the device supports zoom or not.
		 *
		 * \return true if the current camera supports zoom, false otherwise.
		 */
		virtual bool isZoomSupported() const;

		/**
		 * Indicates if the device has a flash or not.
		 *
		 * \return true if the current camera supports flash, false otherwise.
		 */
		virtual bool isFlashSupported() const;

        /**
         * Add an event listener for the snapshot events of this camera.
         *
         * \param listener pointer to CameraSnapshotListener representing the
         * listener that will start receiving snapshot notifications.
         */
        virtual void addSnapshotListener(CameraSnapshotListener* listener);

        /**
         * Remove an event listener for the snapshot events of this camera.
         *
         * \param listener pointer to CameraSnapshotListener representing the
         * listener that will stop receiving snapshot notifications.
         */
        virtual void removeSnapshotListener(CameraSnapshotListener* listener);


        /**
         * This sets the #CameraPreviewDataListener for this camera. Such an
         * listener is needed
         * in order to obtain/manage camera preview data.
         *
         * Note: The last #CameraPreviewDataListener instance set to the camera
         * is the
         * active one.
         *
         * \param listener pointer to a #CameraPreviewDataListener.
         */
		virtual void setCameraPreviewDataListener(
			CameraPreviewDataListener* listener);

		/**
         * Enables events for accessing image data during camera preview.
         *
		 * Note: Only one preview \link #MA_CAMERA_PREVIEW_FRAME event \endlink
		 * may be used at the same time.
		 * This has effect only when the camera preview is started.
		 *
		 * \param previewType a preview event \link #MA_CAMERA_PREVIEW_FRAME type \endlink.
		 *
		 * Note: This defines the moment when a frame will be sent via
		 * #retrievingPreviewFrameFinished of the attached #CameraPreviewDataListener.
		 *
		 * For example if this is #MA_CAMERA_PREVIEW_FRAME then the frame will
		 * be retrieved as soon as possible. In case of MA_CAMERA_PREVIEW_AUTO_FOCUS,
		 * the frame will be retrieved after focus was made.
		 *
		 * \param previewRect a rectangle defining the image frame of the preview data.
         */
        virtual void startRetrievingCameraPreviewData(const int previewType,
			const MARect previewRect);

        /**
         * Turns off the camera preview data events.
         *
         * Note: To make a proper deallocation of camera resources the user must
         * call this method if #startRetrievingCameraPreviewData was used.
         */
        virtual void stopRetrievingCameraPreviewData();

        /**
         * Sets a camera integer property.
         *
         * \param property A string representing which property to set.
         * \param value The integer value which will be assigned to the property.
         *
         * \return one of the following camera return codes:
         * - #MA_CAMERA_RES_OK
         * - #MA_CAMERA_RES_FAILED
         * - #MA_CAMERA_RES_VALUE_NOTSUPPORTED
         * - #MA_CAMERA_RES_INVALID_PROPERTY_VALUE
         */
        virtual int setCameraPropertyInt(
            const MAUtil::String& property,
            const int value);

        /**
         * Sets a camera string property.
         *
         * \param property A string representing which property to set.
         * \param value The string value which will be assigned to the property.
         *
         * \return one of the following camera return codes:
         * - #MA_CAMERA_RES_OK
         * - #MA_CAMERA_RES_FAILED
         * - #MA_CAMERA_RES_VALUE_NOTSUPPORTED
         * - #MA_CAMERA_RES_INVALID_PROPERTY_VALUE
         */
        virtual int setCameraPropertyString(
            const MAUtil::String& property,
            const MAUtil::String& value);

        /**
         * Gets a camera property value as an integer.
         *
         * \param property A string representing which property to set.
         *
         * \return one of the following camera return codes:
         * - #MA_CAMERA_RES_OK
         * - #MA_CAMERA_RES_FAILED
         * - #MA_CAMERA_RES_VALUE_NOTSUPPORTED
         */
        virtual int getCameraPropertyInt(
            const MAUtil::String& property) const;

        /**
         * Gets a camera property as a string, setting also the result code.
         *
         * \param property A string representing which property to set.
		 *
         * \return one of the following camera return codes:
         * - #MA_CAMERA_RES_OK
         * - #MA_CAMERA_RES_FAILED
         * - #MA_CAMERA_RES_VALUE_NOTSUPPORTED
         */
        virtual MAUtil::String getCameraPropertyString(
            const MAUtil::String& property) const;

    protected:
		/**
		 * From CameraListener.
		 *
         * This method is called when there is an camera event.
         *
         * \param event The the camera event.
         */
		void cameraEvent(const MAEvent& event);

		/**
		 * Manages a camera snapshot event. In case the snapshot is
		 * valid or if snapshot timeout was reached, it notifies the
		 * attached snapshot listeners.
		 */
		void manageSnapshotEvent(const MAEvent& snapshotEvent);

		/**
		 * Manages a camera preview event. This will notify the
		 * #CameraPreviewDataListener of the camera that the operation of
		 * retrieving a preview data is finished.
		 */
		void managePreviewEvent(const MAEvent& previewEvent);

    private:
		/**
		 * This notifies the camera snapshot listener when the snapshot
		 * operation failed.
		 *
		 * \param snapshotHandle the handle sent via #takeSnapshot of #Camera,
		 * for which the snapshot has failed.
		 */
		inline void failSnapshot( const MAHandle& snapshotHandle,
			int returnCode );

		/**
		 * This notifies the camera snapshot listener when the snapshot
		 * operation succeeded.
		 *
		 * \param imageData snapshot data that includes the placeholder,
		 * image format or image size.
		 */
		inline void succeedSnapshot( CameraSnapshotData* imageData );

		/**
		 * This notifies the camera preview data listener when the snapshot
		 * operation succeeded.
		 */
		inline void failRetrievingPreviewData( int returnCode,
			const int previewType, const MARect previewRect  );

		/**
		 * Restarts the camera.
		 */
		inline void restartCamera();

		/**
		 * These are used in the snapshot input validation.
		 */
		bool isSnapshotHandleValid( MAHandle dataPlaceholder );
		bool isSnapshotSizeIndexValid( int sizeIndex  );

		/**
		 * Thsere are used in the validation of the preview frame data requests.
		 */
		bool isPreviewTypeValid( int previewType );
		bool isFrameRectValid( MARect frameRect );

		/**
		 * This fills the mSnapshotSizes array with the available snapshot sizes
		 * for the current camera.
		 */
		void fillSnapshotSizesArray();

    private:
		/**
         * Used to dispatch camera snapshot events.
         */
        MAUtil::Vector<CameraSnapshotListener*> mSnapshotListeners;

        /**
         * The listener for the preview data of this camera.
         * Via this pointer the camera frame flow is sent to the user of this API.
         */
        CameraPreviewDataListener* mPreviewDataListenerRef;

        /**
         * The pointer to the preview camera buffer. This is used in the event
         * related to preview frames.
         */
        int* mCameraPreviewBuffer;

        /**
         * The rectangle of the preview frame. This will be sent to the
         * #CameraPreviewDataListener when a preview frame is ready.
         */
        MARect mPreviewRect;

		/**
		 * The current camera index. Index used to distinguish between cameras
		 * (e.q. front or back).
		 */
        unsigned int mCurrentCameraIndex;

		/**
		 * True if camera preview is started, false otherwise.
		 */
		bool mIsCameraStarted;

		/**
		 * Vector containing the available snapshot sizes.
		 */
		MAUtil::Vector<MAExtent> mSnapshotSizes;
	};
} // namespace NativeUI

#endif /* NATIVEUI_CAMERA_H_ */

/*! @} */
