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
 * @file CameraScreen.h
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Screen with camera controls and preview.
 */

#ifndef CAMERASCREEN_H_
#define CAMERASCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/Camera.h>

namespace NativeUI
{
	class RelativeLayout;
	class ImageButton;
	class ActivityIndicator;
}

namespace MoSyncCamera
{
	class CameraScreenObserver
	{
	public:
		/**
		 * The implementation of this method is responsible for
		 * showing the last snapshot taken.
		 *
		 * @param imageDataHandle Snapshot image data handle
		 */
		virtual void displaySnapshot(const MAHandle& imageDataHandle) = 0;
	};

	class CameraScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener,
		public NativeUI::CameraSnapshotListener
	{
	public:
		/**
		 * Constructor for the camera screen.
		 *
		 * @param observer Observer for this screen.
		 */
		CameraScreen(CameraScreenObserver& observer);

		/**
		 *
		 */
		virtual ~CameraScreen();

		/**
		 * From NativeUI::Screen.
		 *
		 * Beside showing this screen it also starts the camera preview.
		 */
		virtual void show();

		/**
		 * From NativeUI::Screen.
		 *
		 * Beside showing this screen with a transition it also starts the
		 * camera preview.
		 */
		virtual int showWithTransition(MAWScreenTransitionType screenTransitionType,
				uint screenTransitionDuration);

        /**
         * This method is called if the touch-up event was inside the
         * bounds of the button.
         * Platform: iOS, Android, Windows Phone.
         *
         * @param button The button object that generated the event.
         */
        virtual void buttonClicked(NativeUI::Widget* button);

        /**
         * CameraSnapshotListener.
         */
        void snapshotFinished( const NativeUI::CameraSnapshotData& imageData );

	private:
        void createUI();

        void setupMainLayout();

        /**
         * Configures the camera and adds it
         * to the main layout.
         */
        void setupCamera();

        /**
		 * Sets the default values for the camera
		 * parameters (e.q. flash, focus etc.).
		 */
        void resetCameraPropeties();

        void setupButtons();

		void setupActivityIndicator();

        void arrangeWidgets();

        /**
         * Changes the state of the button that shows the
         * last snapshot.
         *
         * @param enabled true if button must be enabled,
         * false if it must be disabled.
         */
        void toogleShowSnapshotButton(bool enabled);

        /**
         * Changes the state of the take snapshot button.
         *
         * @param enabled true if button must be enabled,
         * false if it must be disabled.
         */
        void toogleTakeSnapshotButton(bool enabled);

        /**
         * Does everything relate to the operation of taking
         * a snapshot.
         */
        void captureSnapshot();

        /**
         * Activates the next flash mode.
         * It switches between flash modes in a circular manner.
         */
        void activateNextFlashMode();

        /**
         * Activates the next camera.
         * It switches between cameras in a circular manner.
         */
        void activateNextCamera();

        /**
          * Does everything relate to the operation of zooming
          * in.
          */
        void zoomIn();

         /**
           * Does everything relate to the operation of zooming
           * out.
           */
        void zoomOut();

	private:

		CameraScreenObserver& mObserver;

		NativeUI::RelativeLayout* mMainLayout;

		NativeUI::Camera* mCamera;

		NativeUI::ImageButton* mTakeSnapshotButton;

		NativeUI::ImageButton* mShowSnapshotButton;

		NativeUI::ImageButton* mSetNextFlashModeButton;

		NativeUI::ImageButton* mSetNextCameraButton;

		NativeUI::ImageButton* mZoomInButton;

		NativeUI::ImageButton* mZoomOutButton;

		/**
		 * Widget that indicates activity.
		 * Used while taking a snapshot.
		 */
		NativeUI::ActivityIndicator* mActivityIndicator;

		/**
		 * Last taken snapshot handle.
		 */
		MAHandle mLastSnapshotDataHandle;

		uint mCurrentFlashMode;
	};
} // MoSyncCamera
#endif /* CAMERASCREEN_H_ */
