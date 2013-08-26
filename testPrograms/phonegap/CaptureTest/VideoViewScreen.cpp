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
 * @file VideoViewScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a recorded video.
 */

#define PLAY_BUTTON_TEXT "Play"
#define PAUSE_BUTTON_TEXT "Pause"

#include "VideoViewScreen.h"

/**
 * Constructor.
 */
VideoViewScreen::VideoViewScreen() :
	Screen(),
	mVideoView(NULL),
	mPlayBtn(NULL),
	mPauseBtn(NULL)
{
	this->createMainLayout();

	mPlayBtn->addButtonListener(this);
	mPauseBtn->addButtonListener(this);
}

/**
 * Destructor.
 */
VideoViewScreen::~VideoViewScreen()
{
	mPlayBtn->removeButtonListener(this);
	mPauseBtn->removeButtonListener(this);
}

/**
 * Set the video file path.
 * @param path Path to the video file.
 */
void VideoViewScreen::setVideoFilePath(const MAUtil::String& path)
{
//	mVideoView->setPath("sdcard/DCIM/100ANDRO/MOV_0081.mp4");
	mVideoView->setPath(path);
}

/**
 * Creates and adds main layout to the screen.
 */
void VideoViewScreen::createMainLayout()
{
	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);

	mPlayBtn = new Button();
	mPlayBtn->setText(PLAY_BUTTON_TEXT);
	mPlayBtn->fillSpaceHorizontally();
	mainLayout->addChild(mPlayBtn);

	mPauseBtn = new Button();
	mPauseBtn->setText(PAUSE_BUTTON_TEXT);
	mPauseBtn->fillSpaceHorizontally();
	mainLayout->addChild(mPauseBtn);

	mVideoView = new VideoView();
	mVideoView->fillSpaceHorizontally();
	mVideoView->fillSpaceVertically();
//	mVideoView->setWidth(this->getWidth());
//	mVideoView->setHeight(2 * this->getHeight() / 3);
	mainLayout->addChild(mVideoView);

}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void VideoViewScreen::buttonClicked(Widget* button)
{
	if (button == mPlayBtn)
	{
		mVideoView->play();
	}
	else if (button == mPauseBtn)
	{
		mVideoView->pause();
	}
}
