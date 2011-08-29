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
 * @file NativeScreen.cpp
 * @author Emma Tresanszki.
 */

//#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

// Include application's Moblet class.
#include "Moblet.h"

#include "NativeScreen.h"


// Colors used for fonts & background.
enum Colors
{
	SEA_GREEN = 0xC1FFC1 ,
	DARK_SEA_GREEN = 0x9BCD9B ,
	LIGHT_GRAY = 0xD9D9D9 ,
	BLUE       = 0x104E8B
};

/**
 * Constructor.
 */
NativeScreen::NativeScreen() :
	Screen(),
	mMainLayout(NULL),
	mSpacerLayout(NULL),
	mVideoView(NULL),
	mSourceStatus(NULL),
	mButtonsLayout(NULL),
	mPlay(NULL),
	mPause(NULL),
	mStop(NULL),
	mSeekToLayout(NULL),
	mSeekTo(NULL),
	mDuration(NULL),
	mEditBox(NULL),
	mLoadLayout(NULL),
	mSetUrl(NULL),
	mSetPath(NULL),
	mSpacerBottomLayout(NULL),
	mBottomLayout(NULL)
{
	createMainLayout();

//	mVideoView->addVideoViewListener(this);

	mSetUrl->addButtonListener(this);
	mSetPath->addButtonListener(this);
	mPlay->addButtonListener(this);
	mPause->addButtonListener(this);
	mStop->addButtonListener(this);
	mSeekTo->addButtonListener(this);
	mExitButton->addButtonListener(this);
}

/**
 * Destructor.
 */
NativeScreen::~NativeScreen()
{
    mSetUrl->removeButtonListener(this);
    mSetPath->removeButtonListener(this);
    mPlay->removeButtonListener(this);
    mPause->removeButtonListener(this);
    mStop->removeButtonListener(this);
    mSeekTo->removeButtonListener(this);
    mExitButton->removeButtonListener(this);

    mVideoView->removeVideoViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void NativeScreen::createMainLayout()
{
	// Create the main layout (the main widget that holds the other widgets).
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(SEA_GREEN);
	// Arrange the children to center.
	mMainLayout->setChildHorizontalAlignment("center");
	// Add the layout to the screen.
	Screen::setMainWidget(mMainLayout);

	// Add a spacer of 10px before the video view.
	mSpacerLayout = new VerticalLayout();
	mSpacerLayout->setBackgroundColor(BLUE);
	mSpacerLayout->fillSpaceHorizontally();
	mSpacerLayout->setHeight(10);
	mMainLayout->addChild(mSpacerLayout);

	addVideoWidgets();

	// Add another spacer before the bottom area.
	mSpacerBottomLayout = new VerticalLayout();
	mSpacerBottomLayout->setBackgroundColor(BLUE);
	mSpacerBottomLayout->fillSpaceHorizontally();
	mSpacerBottomLayout->setChildHorizontalAlignment("left");
	mSpacerBottomLayout->setHeight(10);
	mMainLayout->addChild(mSpacerBottomLayout);

	// Add Exit button in a horizontal layout.
	mBottomLayout = new HorizontalLayout();
	mBottomLayout->fillSpaceVertically();
	mBottomLayout->fillSpaceHorizontally();

	mExitButton = new Button();
	mExitButton->setText("Exit");
	// Get the screen size.
	MAExtent screenSize = maGetScrSize();
	int screenWidth = EXTENT_X(screenSize);
	mExitButton->setWidth(screenWidth/2);
	// Set gradient background ( applicable only on Android).
//	mExitButton->setBackgroundGradient(0xC1FFC1,0x9BCD9B);
//	mExitButton->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN);
	mBottomLayout->addChild(mExitButton);
	mMainLayout->addChild(mBottomLayout);

}

/**
 * Creates and adds widgets to exemplify video widget.
 */
void NativeScreen::addVideoWidgets()
{
	// Add a label that is refreshed at media source events.
	mSourceStatus = new Label();
	mSourceStatus->setText("Waiting for source");
	mSourceStatus->setFontColor(BLUE);
	mSourceStatus->setFontSize(12);
	mMainLayout->addChild(mSourceStatus);

	// Add 3 buttons in 1 row.
	mButtonsLayout = new HorizontalLayout();
	MAExtent screenSize = maGetScrSize();
	int screenWidth = EXTENT_X(screenSize);
	mPlay = new Button();
	mPlay->setWidth(screenWidth/3);
	mPlay->setText("Play");
	mPlay->setFontColor(BLUE);
	mPause = new Button();
	mPause->setWidth(screenWidth/3);
	mPause->setFontColor(BLUE);
	mPause->setText("Pause");
	mStop = new Button();
	mStop->setWidth(screenWidth/3);
	mStop->setFontColor(BLUE);
	mStop->setText("Stop");
	mButtonsLayout->addChild(mPlay);
	mButtonsLayout->addChild(mPause);
	mButtonsLayout->addChild(mStop);
	mMainLayout->addChild(mButtonsLayout);

	// Add horizontal layout for seek to button and duration label.
	mSeekToLayout = new HorizontalLayout();
	mSeekTo = new Button();
	mSeekTo->setFontColor(BLUE);
	mSeekTo->setText("Seek to 100");
	mDuration = new Label();
	mDuration->setText("Duration: ");
	mSeekToLayout->addChild(mSeekTo);
	mSeekToLayout->addChild(mDuration);
	mMainLayout->addChild(mSeekToLayout);

	// Add the video widget.
	/**
	 * A media controller is attached to this view, and acts differently depending on platform.
	 * On Android typically contains the buttons like "Play/Pause", "Rewind",
	 * Fast Forward" and a progress slider. It takes care of synchronizing the
	 * controls with the state of the MediaPlayer.
	 * The set of controls are in a floating window next to the video widget.
	 * The window will disappear if left idle for three seconds and reappear
	 * when the user touches the video view.
	 */
	mVideoView = new VideoView();
	mVideoView->fillSpaceHorizontally();
	mVideoView->fillSpaceVertically();
//	mVideoView->wrapContentVertically();
	mMainLayout->addChild(mVideoView);

	// Open a test url for Android.
	mVideoView->setPath("/sdcard/bluetooth/heineken.wmv");
//	mVideoView->setURL("http://www.jhepple.com/SampleMovies/niceday.wmv");
	// for iOS: "http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8"

	// Add an edit box for another urls or local paths.
	mEditBox = new EditBox();
	mEditBox->setPlaceholder("Set video uri or path");
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

	// Add layout for load path or uri buttons.
	mLoadLayout = new HorizontalLayout();
	mSetUrl = new Button();
	mSetUrl->setFontColor(BLUE);
	mSetUrl->setText("Load url");
	mSetPath = new Button();
	mSetPath->setFontColor(BLUE);
	mSetPath->setText("Load path");
	mLoadLayout->addChild(mSetUrl);
	mLoadLayout->addChild(mSetPath);
	mMainLayout->addChild(mLoadLayout);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void NativeScreen::buttonClicked(Widget* button)
{
	// When a new url or path is set, the video stops playing the current source
	// and plays the new one.
    if (button == mSetUrl)
    {
        mVideoView->setURL(mEditBox->getText());
    }
    if (button = mSetPath)
    {
		mVideoView->setPath(mEditBox->getText());
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
    else if (button == mSeekTo)
    {
        int result = mVideoView->seekTo(1000);
    }
    else if (button = mExitButton)
    {
		Test::TestMoblet::getInstance()->closeEvent();
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
void NativeScreen::videoViewStateChanged(
    VideoView* videoView,
    const int videoViewState)
{
    if (videoView == mVideoView)
    {
        MAUtil::String text;
        switch (videoViewState)
        {
            case MAW_VIDEO_VIEW_STATE_PLAYING:
                text = "Video is playing";
                break;
            case MAW_VIDEO_VIEW_STATE_PAUSED:
                text = "Video is paused";
                break;
            case MAW_VIDEO_VIEW_STATE_STOPPED:
                text = "Video is stopped";
                break;
            case MAW_VIDEO_VIEW_STATE_SOURCE_READY:
                text = "The video source is ready.";
                // Set the duration.
                mDuration->setText(MAUtil::integerToString(mVideoView->getDuration()));
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
        // Update the source status label.
        mSourceStatus->setText(text);

//        printf("%s", text.c_str());
    }
}
