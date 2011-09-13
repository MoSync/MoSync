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
#include <maprofile.h>

#include "MainScreen.h"
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
	mSeekTo(NULL),
	mCurrentTime(NULL)
{
	createMainLayout();

	mSetUrl->addButtonListener(this);
	mPlay->addButtonListener(this);
	mPause->addButtonListener(this);
	mStop->addButtonListener(this);
	mGetDuration->addButtonListener(this);
	mSeekTo->addButtonListener(this);
	mCurrentTime->addButtonListener(this);

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

    mPlay = new Button();
    mPlay->setText("Play");
    mMainLayout->addChild(mPlay);

	mPause = new Button();
	mPause->setText("Pause");
	mMainLayout->addChild(mPause);

    mStop = new Button();
    mStop->setText("Stop");
    mMainLayout->addChild(mStop);

    mGetDuration = new Button();
    mGetDuration->setText("Get Duration");
    mMainLayout->addChild(mGetDuration);

    HorizontalLayout* layout = new HorizontalLayout();
    mMainLayout->addChild(layout);

    mSeekTo = new Button();
    mSeekTo->setText("Seek to 1000");
    layout->addChild(mSeekTo);

    mCurrentTime = new Button();
    mCurrentTime->setText("Current time");
    layout->addChild(mCurrentTime);

    if (isAndroid())
    {
        mEditBox->setText("http://www.mosync.com/files/videos/heineken.3gp");
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
        mEditBox->setText(buf);
    }
    else if (button == mSeekTo)
    {
        int result = mVideoView->seekTo(1200);
        printf("result seekTo = %d", result);
    }
    else if (button == mCurrentTime)
    {
        int result = mVideoView->currentPlaybackTime();
        printf("result currentPlaybackTime = %d", result);
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
	if (NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
	{
		return true;
	}
	else
	{
		return false;
	}
}
