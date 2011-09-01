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
 * @file VideoScreen.cpp
 * @author Emma Tresanszki.
 *
 * The first screen of the application.
 * It contains widget controlls for Video View: Play, Pause, Stop,
 * an edit box for user input with the source link/path for the video.
 */

#include <mastdlib.h>

// Include application's Moblet class.
#include "Moblet.h"

#include "Util.h"
#include "VideoScreen.h"

/**
 * Constructor.
 */
VideoScreen::VideoScreen() :
	Screen(),
	mMainLayout(NULL),
	mMiddleSpacerLayout(NULL),
	mVideoView(NULL),
	mSourceStatus(NULL),
	mButtonsLayout(NULL),
	mPlay(NULL),
	mPause(NULL),
	mStop(NULL),
	mDuration(NULL),
	mEditBox(NULL),
	mLoadLayout(NULL),
	mSetUrl(NULL),
	mSetPath(NULL),
	mSpacerBottomLayout(NULL)
{
	// Initialize the UI and set the listeners.

	setTitle("Video");

	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	createMainLayout();

	mVideoView->addVideoViewListener(this);
	mPlay->addButtonListener(this);
	mPause->addButtonListener(this);
	mStop->addButtonListener(this);
	mSetUrl->addButtonListener(this);
	mSetPath->addButtonListener(this);
	mExitButton->addButtonListener(this);
}

/**
 * Destructor.
 */
VideoScreen::~VideoScreen()
{
	mVideoView->stop();
    mVideoView->removeVideoViewListener(this);
    mPlay->removeButtonListener(this);
    mPause->removeButtonListener(this);
    mStop->removeButtonListener(this);
    mSetUrl->removeButtonListener(this);
    mSetPath->removeButtonListener(this);
    mExitButton->removeButtonListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void VideoScreen::createMainLayout()
{
	// Create the main layout (the main widget that holds the other widgets).
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(SEA_GREEN);
	// Arrange the children to center.
	mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	// Add the layout to the screen.
	Screen::setMainWidget(mMainLayout);

	addVideoWidgets();

	mMiddleSpacerLayout = new VerticalLayout();
	mMiddleSpacerLayout->setBackgroundColor(SEA_GREEN);
	mMainLayout->addChild(mMiddleSpacerLayout);

	// Add another spacer before the bottom area.
	mSpacerBottomLayout = new VerticalLayout();
	mSpacerBottomLayout->setBackgroundColor(INTENSE_BLUE);
	mSpacerBottomLayout->setHeight(10);
	mMainLayout->addChild(mSpacerBottomLayout);

	mExitButton = new Button();
	mExitButton->setText("Exit");
	mExitButton->setWidth(mScreenWidth/2);
	// Set gradient background ( applicable only on Android).
	mExitButton->setBackgroundGradient(LIGHT_BLUE, INTENSE_BLUE);
	mMainLayout->addChild(mExitButton);
}

/**
 * Create the controls area with Play, Pause and Stop buttons.
 */
void VideoScreen::createPlaybackButtons()
{
	/**
	 * Add 3 buttons in 1 row: Play, Pause and stop.
	 * All buttons are disabled by default, until source can be played.
	 */
	mButtonsLayout = new HorizontalLayout();
	mButtonsLayout->wrapContentVertically();
	mButtonsLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);

	mPlay = new Button();
	// Apply a gradient ( on Android, or blank color on iOS).
	if ( mPlay->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN) != MAW_RES_OK )
	{
		mPlay->setBackgroundColor(DARK_SEA_GREEN);
	}
	mPlay->setText("Play");
	mPlay->setEnabled(false);
	mPlay->setFontColor(INTENSE_BLUE);

	mPause = new Button();
	// Apply a gradient ( on Android, or blank color on iOS).
	if ( mPause->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN) != MAW_RES_OK )
	{
		mPause->setBackgroundColor(DARK_SEA_GREEN);
	}
	mPause->setFontColor(INTENSE_BLUE);
	mPause->setEnabled(false);
	mPause->setText("Pause");

	mStop = new Button();
	mStop->setEnabled(false);
	// Apply a gradient ( on Android, or blank color on iOS).
	if ( mStop->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN) != MAW_RES_OK )
	{
		mStop->setBackgroundColor(DARK_SEA_GREEN);
	}
	mStop->setFontColor(INTENSE_BLUE);
	mStop->setText("Stop");

	HorizontalLayout* playLayout = new HorizontalLayout();
	playLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	playLayout->setWidth(mScreenWidth/3);
	playLayout->addChild(mPlay);

	HorizontalLayout* pauseLayout = new HorizontalLayout();
	pauseLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	pauseLayout->setWidth(mScreenWidth/3);
	pauseLayout->addChild(mPause);

	HorizontalLayout* stopLayout = new HorizontalLayout();
	stopLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	stopLayout->setWidth(mScreenWidth/3);
	stopLayout->addChild(mStop);

	mButtonsLayout->addChild(playLayout);
	mButtonsLayout->addChild(pauseLayout);
	mButtonsLayout->addChild(stopLayout);

	mMainLayout->addChild(mButtonsLayout);
}

/**
 * Creates and adds widgets to exemplify video widget.
 */
void VideoScreen::addVideoWidgets()
{
	// Add a label that is refreshed at media source events.
	mSourceStatus = new Label();
	mSourceStatus->setText(SOURCE_LOADING);
	mSourceStatus->setFontColor(INTENSE_BLUE);
	mSourceStatus->setFontSize(12);
	mMainLayout->addChild(mSourceStatus);

	// Add a spacer of 10px before the buttons.
	mTopSpacerLayout = new VerticalLayout();
	mTopSpacerLayout->setBackgroundColor(INTENSE_BLUE);
	mTopSpacerLayout->setHeight(10);
	mMainLayout->addChild(mTopSpacerLayout);

	createPlaybackButtons();

	// Add duration label.
	mDuration = new Label();
	mDuration->setFontColor(INTENSE_BLUE);
	mDuration->setText("Video Duration");
	mMainLayout->addChild(mDuration);

	// Add the video widget.
	/**
	 * A media controller is attached to this view, and acts differently
	 * depending on platform.
	 * On Android typically contains the buttons like "Play/Pause", "Rewind",
	 * Fast Forward" and a progress slider. It takes care of synchronizing the
	 * controls with the state of the MediaPlayer.
	 * The set of controls are in a floating window next to the video widget.
	 * The window will disappear if left idle for three seconds and reappear
	 * when the user touches the video view.
	 */
	mVideoView = new VideoView();
	mVideoView->fillSpaceHorizontally();
	mVideoView->setHeight(mScreenHeight/3);
	mVideoView->setWidth(MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	mMainLayout->addChild(mVideoView);

	// Add an edit box for another urls or local paths.
	mEditBox = new EditBox();
//	mEditBox->setPlaceholder("Set video uri or path");
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

	// Add layout for load path or uri buttons.
	mLoadLayout = new HorizontalLayout();
	mLoadLayout->wrapContentVertically();

	mSetUrl = new Button();
	mSetUrl->setFontColor(INTENSE_BLUE);
	// Apply a gradient ( on Android, or blank color on iOS).
	if ( mSetUrl->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN) != MAW_RES_OK )
	{
		mSetUrl->setBackgroundColor(DARK_SEA_GREEN);
	}
	mSetUrl->setText("Load url");

	mSetPath = new Button();
	mSetPath->setFontColor(INTENSE_BLUE);
	// Apply a gradient.
	if ( mSetPath->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN) != MAW_RES_OK )
	{
		mSetPath->setBackgroundColor(DARK_SEA_GREEN);
	}
	mSetPath->setText("Load path");

	HorizontalLayout* urlLayout = new HorizontalLayout();
	urlLayout->setWidth(mScreenWidth/2);
	urlLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	urlLayout->addChild(mSetUrl);

	HorizontalLayout* pathLayout = new HorizontalLayout();
	pathLayout->setWidth(mScreenWidth/2);
	pathLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	pathLayout->addChild(mSetPath);

	mLoadLayout->addChild(urlLayout);
	mLoadLayout->addChild(pathLayout);

	mMainLayout->addChild(mLoadLayout);
}

/**
 * Load the default url. This can be set from SettingsScreen.
 */
void VideoScreen::loadDefaultUrl(const MAUtil::String url)
{
	mEditBox->setText(url);
	mVideoView->setURL(url);
}

/**
 * Show or hide the duration label.
 * @param state if true show it, hide it otherwise.
 */
void VideoScreen::showDurationLabel(bool state)
{
	mDuration->setVisible(state);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void VideoScreen::buttonClicked(Widget* button)
{
	// When a new url or path is set, the video stops playing the current source
	// and plays the new one.
    if (button == mPlay)
    {
        mVideoView->play();
        // Display the duration label.
        mDuration->setText(
			"Duration: " +
			MAUtil::integerToString(mVideoView->getDuration()));
    }
    else if (button == mPause)
    {
        mVideoView->pause();
    }
    else if (button == mStop)
    {
        mVideoView->stop();
    }
    else if (button == mSetUrl)
    {
        mVideoView->setURL(mEditBox->getText());
    }
    else if (button = mSetPath)
    {
		mVideoView->setPath(mEditBox->getText());
    }
    else if (button = mExitButton)
    {
		Test::VideoMoblet::getInstance()->closeEvent();
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
void VideoScreen::videoViewStateChanged(
    VideoView* videoView,
    const int videoViewState)
{
    if (videoView == mVideoView)
    {
        MAUtil::String text;
        switch (videoViewState)
        {
            case MAW_VIDEO_VIEW_STATE_PLAYING:
                text = SOURCE_PLAYING;
                break;
            case MAW_VIDEO_VIEW_STATE_PAUSED:
                text = SOURCE_PAUSED;
                break;
            case MAW_VIDEO_VIEW_STATE_STOPPED:
                text = SOURCE_STOPPED;
                break;
            case MAW_VIDEO_VIEW_STATE_SOURCE_READY:
                text = SOURCE_READY;
                // Only now source can be played.
                mPlay->setEnabled(true);
                mPause->setEnabled(true);
                mStop->setEnabled(true);
                break;
            case MAW_VIDEO_VIEW_STATE_FINISHED:
                text = SOURCE_FINISHED;
                break;
            case MAW_VIDEO_VIEW_STATE_INTERRUPTED:
                text = SOURCE_ERROR;
                // Disable the control buttons.
                mPlay->setEnabled(false);
                mPause->setEnabled(false);
                mStop->setEnabled(false);
                break;
            default:
                text = "Unknown event type for video view";
        }
        // Update the source status label.
        mSourceStatus->setText(text);
    }
}
