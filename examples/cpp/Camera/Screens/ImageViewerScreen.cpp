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
 * @file ImageViewerScreen.cpp
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Screen that displays snapshot taken with Camera.
 */
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

#include "ImageViewerScreen.h"

// This is needed in order to access resources from the res.lstx
#include "MAHeaders.h"

#include "ScreenUtils.h"

#define CONTAINER_BUTTON_RATIO 0.16 // Image button/container width ratio for normal buttons.

using namespace NativeUI;

namespace MoSyncCamera
{
	ImageViewerScreen::ImageViewerScreen(ImageViewerScreenObserver& observer):
			mObserver(observer),
			mMainLayout(NULL),
			mImageView(NULL),
			mDismissButton(NULL),
			mSaveImageButton(NULL),
			mImageHandle(0)
	{
		createUI();
	}

	ImageViewerScreen::~ImageViewerScreen()
	{
		mDismissButton->removeButtonListener(this);
		mSaveImageButton->removeButtonListener(this);
		if ( mImageHandle > 0 )
		{
			maDestroyPlaceholder(mImageHandle);
		}
	}

	void ImageViewerScreen::createUI()
	{
		setupMainLayout();
		setupImageView();
		setupButtons();
		arrangeWidgets();
	}

	void ImageViewerScreen::setupMainLayout()
	{
		mMainLayout = new RelativeLayout();
		mMainLayout->setBackgroundColor(BLACK_COLOR);
		setMainWidget(mMainLayout);
	}

	void ImageViewerScreen::setupImageView()
	{
		mImageView = new Image();
		mImageView->fillSpaceHorizontally();
		mImageView->fillSpaceVertically();
		mImageView->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
		mMainLayout->addChild(mImageView);
		mImageView->setVisible(false);
	}

	void ImageViewerScreen::setupButtons()
	{
		// dismiss button
		mDismissButton = new NativeUI::ImageButton();
		ScreenUtils::setupImageButton(mDismissButton,
				this, RES_BACK_IMG, RES_BACK_IMG_PRESSED);
		mMainLayout->addChild(mDismissButton);

		// save button
		mSaveImageButton = new NativeUI::ImageButton();
		ScreenUtils::setupImageButton(mSaveImageButton,
				this, RES_SAVE_IMG, RES_SAVE_IMG_PRESSED);
		mMainLayout->addChild(mSaveImageButton);
	}

	void ImageViewerScreen::arrangeWidgets()
	{
		/// Get screen dimensions.
		MAExtent size = maGetScrSize();
		/// Extract the screen width
		int screenWidth = EXTENT_X(size);
		/// Extract the screen height
		int screenHeight = EXTENT_Y(size);

		mImageView->setPosition(0, 0);
		mImageView->setWidth(screenWidth);
		mImageView->setHeight(screenHeight);

		ScreenUtils::resizeWidget(mDismissButton, size, CONTAINER_BUTTON_RATIO);
		// bottom left
		int btnY = screenHeight - mDismissButton->getHeight();
		int btnX = 0;
		mDismissButton->setPosition(btnX, btnY);

		ScreenUtils::resizeWidget(mSaveImageButton, size, CONTAINER_BUTTON_RATIO);
		 // bottom right
		btnY = screenHeight - mSaveImageButton->getHeight();
		btnX = screenWidth - mSaveImageButton->getWidth();
		mSaveImageButton->setPosition(btnX, btnY);
	}

	void ImageViewerScreen::buttonClicked(NativeUI::Widget* button)
	{
		if ( button == mDismissButton )
		{
			mObserver.dismissSnapshot();
			if ( mImageHandle > 0 )
			{
				maDestroyPlaceholder(mImageHandle);
				mImageHandle = 0;
			}
		}
		else if ( button == mSaveImageButton )
		{
			maAlert("Save image", "Image saving is not yet available", "OK", NULL, NULL);
		}
	}

	void ImageViewerScreen::setImageWithData(const MAHandle& imageDataHandle)
	{
		if ( imageDataHandle != mImageHandle )
		{
			mImageHandle = maCreatePlaceholder();

			maCreateImageFromData(
				mImageHandle,
				imageDataHandle,
				0,
				maGetDataSize(imageDataHandle));

			mImageView->setImage(mImageHandle);
			mImageView->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
			mImageView->setVisible(true);
		}
	}
} // MoSyncCamera
