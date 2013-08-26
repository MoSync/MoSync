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
    ImageViewerScreen::ImageViewerScreen(ImageViewerScreenController& controller):
            mController(controller),
            mMainLayout(NULL),
            mImageView(NULL),
            mDismissButton(NULL),
            mSaveImageButton(NULL),
            mActivityIndicator(NULL)
    {
        createUI();
    }

    ImageViewerScreen::~ImageViewerScreen()
    {
        mDismissButton->removeButtonListener(this);
        mSaveImageButton->removeButtonListener(this);
    }

    void ImageViewerScreen::createUI()
    {
        setupMainLayout();
        setupImageView();
        setupButtons();
        setupActivityIndicator();
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

    void ImageViewerScreen::setupActivityIndicator()
    {
        mActivityIndicator = new ActivityIndicator();
        mActivityIndicator->setWidth(ACTIVITY_INDICATOR_WIDTH);
        mActivityIndicator->setHeight(ACTIVITY_INDICATOR_HEIGHT);
        mActivityIndicator->hide();
        mMainLayout->addChild(mActivityIndicator);
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

        mActivityIndicator->setPosition((screenWidth - mActivityIndicator->getWidth())/2,
                (screenHeight - mActivityIndicator->getHeight())/2);

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
            mController.imageViewingDone();
        }
        else if ( button == mSaveImageButton )
        {
            mController.exportImageToGalleryRequested();
        }
    }

    void ImageViewerScreen::setImageWithData(const MAHandle& imageHandle)
    {
        mImageView->setImage(imageHandle);
        mImageView->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
        mImageView->setVisible(true);
    }


    void ImageViewerScreen::toggleImageSavingInProgress(bool isInProgress)
    {
        toogleBackButton(!isInProgress);
        toogleSaveSnapshotButton(!isInProgress);

        if ( isInProgress )
        {
            mActivityIndicator->show();
        }
        else
        {
            mActivityIndicator->hide();
        }
    }


    void ImageViewerScreen::toogleBackButton(bool enabled)
	{
		if ( enabled )
		{
			// Use enabled images
			mDismissButton->setBackgroundImage(RES_BACK_IMG);
			mDismissButton->setPressedImage(RES_BACK_IMG_PRESSED);
		}
		else
		{
			// Use disabled images
			mDismissButton->setBackgroundImage(RES_BACK_IMG_DISABLED);
		}
		mDismissButton->setEnabled(enabled);
    }


    void ImageViewerScreen::toogleSaveSnapshotButton(bool enabled)
	{
		if ( enabled )
		{
			// Use enabled images
			mSaveImageButton->setBackgroundImage(RES_SAVE_IMG);
			mSaveImageButton->setPressedImage(RES_SAVE_IMG_PRESSED);
		}
		else
		{
			// Use disabled images
			mSaveImageButton->setBackgroundImage(RES_SAVE_IMG_DISABLED);
		}
		mSaveImageButton->setEnabled(enabled);
    }
} // MoSyncCamera
