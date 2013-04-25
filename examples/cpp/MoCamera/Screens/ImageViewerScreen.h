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
	class ImageViewerScreenObserver
	{
	public:
		/**
		 * Notifies the observer that the image viewer screen is ready
		 * to be dismissed.
		 */
		virtual void imageViewingDone() = 0;

		/**
		 * Notifies the observer that a request for saving
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
		 * @param observer Observer for this screen.
		 */
		ImageViewerScreen(ImageViewerScreenObserver& observer);

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
         * Makes changes to the UI needed when image saving
         * is in progress.
         */
        void showImageSavingInProgress();

        /**
         * Makes changes to the UI needed when image saving is
         * completed.
         *
         * @param imageSaved True if image was saved in the
         * phone library, false otherwise.
         */
        void hideImageSavingInProgress(bool imageSaved);


	private:

        void createUI();

        void setupMainLayout();

        void setupImageView();

        void setupButtons();

		void setupActivityIndicator();

        void arrangeWidgets();

	private:

        ImageViewerScreenObserver& mObserver;

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
