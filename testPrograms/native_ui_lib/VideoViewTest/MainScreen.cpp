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
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <mastring.h>		// C string functions

#include "MainScreen.h"

#define BUF_MAX 256

// Text for video control button.
#define SHOW_VIDEO_CONTROL_BUTTON_TEXT "Show video control"
#define HIDE_VIDEO_CONTROL_BUTTON_TEXT "Hide video control"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mVideoView(NULL),
	mEditBox(NULL),
	mSetUrl(NULL),
	mPlay(NULL),
	mPause(NULL),
	mStop(NULL),
	mGetDuration(NULL),
	mDuration(NULL),
	mSeekTo(NULL),
	mCurrentTime(NULL),
	mTime(NULL),
	mVideoControl(NULL)
{
	createMainLayout();

	mSetUrl->addButtonListener(this);
	mPlay->addButtonListener(this);
	mPause->addButtonListener(this);
	mStop->addButtonListener(this);
	mGetDuration->addButtonListener(this);
	mSeekTo->addButtonListener(this);
	mCurrentTime->addButtonListener(this);
	mVideoControl->addButtonListener(this);

	mVideoView->addVideoViewListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mSetUrl->removeButtonListener(this);
    mPlay->removeButtonListener(this);
    mPause->removeButtonListener(this);
    mStop->removeButtonListener(this);
    mGetDuration->removeButtonListener(this);
    mSeekTo->removeButtonListener(this);
    mCurrentTime->removeButtonListener(this);
    mVideoControl->removeButtonListener(this);

    mVideoView->removeVideoViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mVideoView = new VideoView();
	mVideoView->setWidth(200);
	mVideoView->setHeight(200);
	mMainLayout->addChild(mVideoView);

	mEditBox = new EditBox();
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

	mSetUrl = new Button();
	mSetUrl->setText("Set url");

	mMainLayout->addChild(mSetUrl);

	HorizontalLayout* controlsLayout = new HorizontalLayout();
	mMainLayout->addChild(controlsLayout);
    mPlay = new Button();
    mPlay->setText("Play");
    mPlay->fillSpaceHorizontally();
    controlsLayout->addChild(mPlay);

	mPause = new Button();
	mPause->setText("Pause");
	mPause->fillSpaceHorizontally();
	controlsLayout->addChild(mPause);

    mStop = new Button();
    mStop->setText("Stop");
    mStop->fillSpaceHorizontally();
    controlsLayout->addChild(mStop);

    mSeekTo = new Button();
    mSeekTo->setText("Seek to 1000");
    mMainLayout->addChild(mSeekTo);

    HorizontalLayout* durationlayout = new HorizontalLayout();
    mMainLayout->addChild(durationlayout);
    mGetDuration = new Button();
    mGetDuration->setText("Get Duration");
    durationlayout->addChild(mGetDuration);

    mDuration = new Label();
    mDuration->setText("The duration");
    durationlayout->addChild(mDuration);

    mVideoControl = new Button();
    mVideoControl->setText(HIDE_VIDEO_CONTROL_BUTTON_TEXT);
    mMainLayout->addChild(mVideoControl);

    HorizontalLayout* timeLayout = new HorizontalLayout();
    mMainLayout->addChild(timeLayout);

    mCurrentTime = new Button();
    mCurrentTime->setText("Current time");
    timeLayout->addChild(mCurrentTime);

    mTime = new Label();
    mTime->setText("Current Time");
    timeLayout->addChild(mTime);

    if (isAndroid())
    {
        mEditBox->setText("http://www.mosync.com/files/videos/Video.3gp");
    }
    else
    {
        mEditBox->setText("http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8");
    }
    mVideoView->setURL(mEditBox->getText());
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mSetUrl)
    {
        mVideoView->setURL(mEditBox->getText());
    }
    else if (button == mPlay)
    {
        mVideoView->play();
    }
    else if (button == mPause)
    {
        mVideoView->pause();
    }
    else if (button == mStop)
    {
        mVideoView->stop();
    }
    else if (button == mGetDuration)
    {
        int duration = mVideoView->getDuration();
        char buf[256];
        itoa(duration, buf, 10);
        mDuration->setText(buf);
    }
    else if (button == mSeekTo)
    {
        int result = mVideoView->seekTo(1200);
        printf("result seekTo = %d", result);
    }
    else if (button == mCurrentTime)
    {
        int result = mVideoView->currentPlaybackTime();
        mTime->setText(MAUtil::integerToString(result));
        printf("result currentPlaybackTime = %d", result);
    }
    else if (button == mVideoControl)
    {
        this->handleVideoControlButtonClicked();
    }

}

/**
 * This method is called when the state of the video has changed.
 * @param videoView The video view object that generated the event.
 * @param videoViewState The new state of the video.
 * Any of the following constants:
 * - #MAW_VIDEO_VIEW_STATE_PLAYING the video is playing.
 * - #MAW_VIDEO_VIEW_STATE_PAUSED the video is paused.
 * - #MAW_VIDEO_VIEW_STATE_STOPPED the video is stopped.
 * - #MAW_VIDEO_VIEW_STATE_SOURCE_READY the source is loaded into memory.
 * - #MAW_VIDEO_VIEW_STATE_FINISHED the video has finished playing.
 * - #MAW_VIDEO_VIEW_STATE_INTERRUPTED Playback is temporarily
 * interruped(maybe there's no data in the buffer).
 */
void MainScreen::videoViewStateChanged(
    VideoView* videoView,
    const int videoViewState)
{
    if (videoView == mVideoView)
    {
        MAUtil::String text;
        switch (videoViewState)
        {
            case MAW_VIDEO_VIEW_STATE_PLAYING:
                text = "The video is playing";
                break;
            case MAW_VIDEO_VIEW_STATE_PAUSED:
                text = "The video is paused";
                break;
            case MAW_VIDEO_VIEW_STATE_STOPPED:
                text = "The video is stopped";
                break;
            case MAW_VIDEO_VIEW_STATE_SOURCE_READY:
                text = "The video source is ready.";
                break;
            case MAW_VIDEO_VIEW_STATE_FINISHED:
                text = "The video has finished playing";
                break;
            case MAW_VIDEO_VIEW_STATE_INTERRUPTED:
                text = "Some error occurred on the video view";
                break;
            default:
                text = "Unknown event type for video view";
        }

        printf("%s", text.c_str());
    }
}

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool MainScreen::isAndroid()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);

	if(strcmp(platform, "Android") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Handle the click event for video control button.
 * Show/hide the video control and change button's text.
 */
void MainScreen::handleVideoControlButtonClicked()
{
	if (strcmp(mVideoControl->getText().c_str(), HIDE_VIDEO_CONTROL_BUTTON_TEXT) == 0)
	{
		mVideoView->hideControl();
		mVideoControl->setText(SHOW_VIDEO_CONTROL_BUTTON_TEXT);
	}
	else
	{
		mVideoView->showControl();
		mVideoControl->setText(HIDE_VIDEO_CONTROL_BUTTON_TEXT);
	}

	bool value = mVideoView->isControlVisible();
	printf("MainScreen::handleVideoControlButtonClicked - control visible = %d",
		value);
}
