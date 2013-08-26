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
	class CameraScreenController
	{
	public:
		/**
		 * Notifies that a request for displaying an snapshot was made.
		 */
		virtual void snapshotDisplayRequested() = 0;

		/**
		 * Notifies that a request for snapshot was made.
		 */
		virtual void snapshotRequested() = 0;
	};

	class CameraScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor for the camera screen.
		 *
		 * @param controller Controller of this screen.
		 */
		CameraScreen(CameraScreenController& controller);

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
		 * This method activates the snapshot operation.
		 *
		 * @param imageDataHandle place-holder where the snapshot data will be
		 * written
		 */
		void takeSnapshot(MAHandle imageDataHandle);

		/**
		 * Registers a snapshot listener to the camera own by this screen.
		 *
		 * @param cameraListener The listener that will start receiving snapshot
		 * notifications
		 */
		void registerCameraListener(NativeUI::CameraSnapshotListener* cameraListener);

		/**
		 * Unregisters a snapshot listener to the camera own by this screen.
		 *
		 * @param cameraListener The listener that will stop receiving snapshot
		 * notifications
		 */
		void unregisterCameraListener(NativeUI::CameraSnapshotListener* cameraListener);

		/**
		 * Makes changes to the UI needed when a snapshot is in progress.
		 */
		void showSnapshotInProgress();

		/**
		 * Makes changes to the UI needed when a snapshot is in not in progress.
		 */
		void hideSnapshotInProgress(bool snapshotIsValid);

		/**
		 * Triggers auto focus.
		 * This is for the devices that do not have continuous auto focus.
		 */
		void triggerAutoFocus();

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
        void resetCameraProperties();

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
		 * Changes the state of the button that captures a
		 * snapshot
		 *
		 * @param enabled true if button must be enabled,
		 * false if it must be disabled.
		 */
        void toogleTakeSnapshotButton(bool enabled);

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
         * Handlers for zoom functionalities
         */
        void zoomIn();
        void zoomOut();

        /**
         * Starts camera preview, resets the camera values
         * and setups other UI related functionalities.
         */
        void startCameraPreview();

	private:

		CameraScreenController& mController;

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

		uint mCurrentFlashMode;
	};
} // MoSyncCamera
#endif /* CAMERASCREEN_H_ */
