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
 * @file ImageViewerScreen.h
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Screen that displays an image.
 */

#ifndef SNAPSHOTSCREEN_H_
#define SNAPSHOTSCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ButtonListener.h>

namespace NativeUI
{
	class RelativeLayout;
	class ImageButton;
	class Image;
}

namespace MoSyncCamera
{
	class ImageViewerScreenController
	{
	public:
		/**
		 * Notifies the controller that the image viewer screen is ready
		 * to be dismissed.
		 */
		virtual void imageViewingDone() = 0;

		/**
		 * Notifies the controller that a request for saving
		 * the displayed image was made.
		 */
		virtual void exportImageToGalleryRequested() = 0;
	};

	class ImageViewerScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param controller Controller of this screen.
		 */
		ImageViewerScreen(ImageViewerScreenController& controller);

		~ImageViewerScreen();

        /**
         * This method is called if the touch-up event was inside the
         * bounds of the button.
         * Platform: iOS, Android, Windows Phone.
         * @param button The button object that generated the event.
         */
        virtual void buttonClicked(NativeUI::Widget* button);

        /**
		 * Sets the displayed image using the given data.
		 * @param imageDataHandle Snapshot image data handle.
		 */
        void setImageWithData(const MAHandle& imageDataHandle);

        /**
         * Makes changes to the UI related to the process of saving an
         * image to photo gallery.
         *
         * @param imageSaved True will change the UI suck it suggests
         * that the operation of saving an image is in progress,
         * false will restore the normal/idle state.
         */
        void toggleImageSavingInProgress(bool isInProgress);

        /**
		 * Changes the state of the button that dismisses this screen.
		 *
		 * @param enabled true if button must be enabled,
		 * false if it must be disabled.
		 */
        void toogleBackButton(bool enabled);

        /**
		 * Changes the state of the button that exports the snapshot
		 * to the device gallery.
		 *
		 * @param enabled true if button must be enabled,
		 * false if it must be disabled.
		 */
        void toogleSaveSnapshotButton(bool enabled);

	private:

        void createUI();

        void setupMainLayout();

        void setupImageView();

        void setupButtons();

		void setupActivityIndicator();

        void arrangeWidgets();

	private:

        ImageViewerScreenController& mController;

		NativeUI::RelativeLayout* mMainLayout;

		NativeUI::Image* mImageView;

		NativeUI::ImageButton* mDismissButton;

		NativeUI::ImageButton* mSaveImageButton;

		/**
		 * Widget that indicates activity.
		 * Used while taking a exporting an image to photo
		 * gallery.
		 */
		NativeUI::ActivityIndicator* mActivityIndicator;
	};
} // MoSyncCamera
#endif /* SNAPSHOTSCREEN_H_ */
