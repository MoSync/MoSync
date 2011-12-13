/*
Copyright (C) 2011 MoSync AB

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
	mVideoView->setPath(path);
}

/**
 * Creates and adds main layout to the screen.
 */
void VideoViewScreen::createMainLayout()
{
	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);

	mVideoView = new VideoView();
	mVideoView->setWidth(this->getWidth());
	mVideoView->setHeight(2 * this->getHeight() / 3);
	mainLayout->addChild(mVideoView);

	mPlayBtn = new Button();
	mPlayBtn->setText(PLAY_BUTTON_TEXT);
	mPlayBtn->fillSpaceHorizontally();
	mainLayout->addChild(mPlayBtn);

	mPauseBtn = new Button();
	mPauseBtn->setText(PAUSE_BUTTON_TEXT);
	mPauseBtn->fillSpaceHorizontally();
	mainLayout->addChild(mPauseBtn);
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
