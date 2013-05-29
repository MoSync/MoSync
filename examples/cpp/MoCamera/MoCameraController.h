/*
 Copyright (C) 2013 MoSync AB

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
 * @file MoCameraController.h
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Application's main controller.
 */

#ifndef MOSYNCCAM_CONTROLLER_H_
#define MOSYNCCAM_CONTROLLER_H_

#include "Screens/CameraScreen.h"
#include "Screens/ImageViewerScreen.h"

#include "MAUtil/Environment.h"
#include "NativeUI/Camera.h"

namespace MoSyncCamera
{
	class MoCameraController:
		public CameraScreenController,
		public ImageViewerScreenController,
		public NativeUI::CameraSnapshotListener,
		public MAUtil::MediaExportListener,
		public MAUtil::PointerListener
	{
	public:
		MoCameraController();

		virtual ~MoCameraController();

		/**
		 * Shows the first screen of the application.
		 */
		void displayMainScreen();

		/**
		 * When notified of this request it
		 * displays a snapshot on a separate screen.
		 *
		 * From CameraScreenController
		 *
		 * @param imageDataHandle Snapshot image data handle.
		 */
		void snapshotDisplayRequested();

		/**
		 * When notified of this request it
		 * triggers a snapshot operation.
		 *
		 * From CameraScreenController
		 */
		void snapshotRequested();

		/**
		 * When notified that the image viewer screen is done it
		 * goes back to the camera screen.
		 *
		 * From ImageViewerScreenController
		 */
		void imageViewingDone();

		/**
		 * When notified of this the image from the ImageViewerScreen
		 * will be saved in the photo library of the device.
		 *
		 * From ImageViewerScreenController
		 */
		void exportImageToGalleryRequested();

        /**
         * Handles the snapshot operation result.
         *
         * From CameraSnapshotListener.
         */
        void snapshotFinished( const NativeUI::CameraSnapshotData& imageData );

        /**
         * Handles the image export operation result.
         *
         * From MediaExportListener.
         */
        void imageExportToGalleryFinished( const MAHandle& imageHandle,
			int resultCode);

        /**
         * Handles the hardware back press.
         */
        void handleBackPress();

        /**
         * Pointer event listeners. Used here to trigger auto focus.
         *
         * From PointerListener
         */
        virtual void pointerPressEvent(MAPoint2d point);
        virtual void pointerMoveEvent(MAPoint2d point);
        virtual void pointerReleaseEvent(MAPoint2d point);

	private:
		/**
		 * Sets the screen transition used when showing/dismissing
		 * a screen.
		 */
		void setScreenTransitions();

		/**
		 * Retains the displayed screen handle.
		 *
		 * @param currentScreen The screen that will be displayed.
		 */
		void setCurrentScreen( const NativeUI::Screen& currentScreen );

		/**
		 * Returns if the screen given as parameter is displayed.
		 *
		 * @param currentScreen the screen that will be verified.
		 *
		 * @return true if the screen is already displayed, false otherwise.
		 */
		bool isDisplayed( const NativeUI::Screen& currentScreen );

	private:
		CameraScreen* mCameraScreen;

		ImageViewerScreen* mImageViewerScreen;

		/**
		 * Transition type used when displaying
		 * screens.
		 */
		MAWScreenTransitionType mForwardTransition;

		/**
		 * Transition type used when dismissing screens.
		 */
		MAWScreenTransitionType mBackwardTransition;

		/**
		 * Used to prevent displaying the same screen twice.
		 */
		MAHandle mDisplayedScreen;

		/**
		 * Handle for the image displayed.
		 */
		MAHandle mDisplayedImageHandle;

		/**
		 * Last taken snapshot handle
		 */
		MAHandle mLastSnapshotDataHandle;
	};
} // MoSyncCamera

#endif /* MOSYNCCAM_CONTROLLER_H_ */
