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
	mTopSpacerLayout(NULL),
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
	mMiddleSpacerLayout(NULL),
	mSpacerBottomLayout(NULL)
{
	// Initialize the UI and set the listeners.

	setTitle("Video");

	createMainLayout();

	// Loading local videos is available only on Android.
	if ( !isAndroid() )
	{
		mSetPath->setVisible(false);
	}

	mVideoView->addVideoViewListener(this);
	mEditBox->addEditBoxListener(this);
	mPlay->addButtonListener(this);
	mPause->addButtonListener(this);
	mStop->addButtonListener(this);
	mSetUrl->addButtonListener(this);
	mSetPath->addButtonListener(this);
}

/**
 * Destructor.
 */
VideoScreen::~VideoScreen()
{
//	mVideoView->stop();
    mVideoView->removeVideoViewListener(this);
	mEditBox->removeEditBoxListener(this);
    mPlay->removeButtonListener(this);
    mPause->removeButtonListener(this);
    mStop->removeButtonListener(this);
    mSetUrl->removeButtonListener(this);
    mSetPath->removeButtonListener(this);
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
	setMainWidget(mMainLayout);

	addVideoWidgets();

	mMiddleSpacerLayout = new VerticalLayout();
	mMiddleSpacerLayout->setBackgroundColor(SEA_GREEN);
	mMainLayout->addChild(mMiddleSpacerLayout);

	// Add another spacer before the bottom area.
	mSpacerBottomLayout = new VerticalLayout();
	mSpacerBottomLayout->setBackgroundColor(INTENSE_BLUE);
	mSpacerBottomLayout->setHeight(10);
	mMainLayout->addChild(mSpacerBottomLayout);
}

/**
 * Create the controls area with Play, Pause and Stop buttons.
 */
void VideoScreen::createPlaybackButtons()
{
	/**
	 * Add 3 buttons in 1 row: Play, Pause and stop.
	 * Pause and Stop are disabled by default, until source is playing.
	 */
	mButtonsLayout = new HorizontalLayout();
	mButtonsLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);

	mPlay = new Button();
	// Apply a background gradient on Android.
	mPlay->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN);
	mPlay->setText("Play");
	mPlay->setFontColor(INTENSE_BLUE);

	mPause = new Button();
	// Apply a background gradient on Android.
	mPause->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN);
	mPause->setFontColor(INTENSE_BLUE);
	mPause->setEnabled(false);
	mPause->setText("Pause");

	mStop = new Button();
	mStop->setEnabled(false);
	// Apply a background gradient on Android.
	mStop->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN);
	mStop->setFontColor(INTENSE_BLUE);
	mStop->setText("Stop");

	HorizontalLayout* playLayout = new HorizontalLayout();
	playLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	playLayout->setWidth(getScreenWidth()/3);
	playLayout->addChild(mPlay);

	HorizontalLayout* pauseLayout = new HorizontalLayout();
	pauseLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	pauseLayout->setWidth(getScreenWidth()/3);
	pauseLayout->addChild(mPause);

	HorizontalLayout* stopLayout = new HorizontalLayout();
	stopLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	stopLayout->setWidth(getScreenWidth()/3);
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

	// Add layout for load path or uri buttons.
	mLoadLayout = new HorizontalLayout();

	mSetUrl = new Button();
	mSetUrl->setFontColor(INTENSE_BLUE);
	// Apply a gradient on Android.
	mSetUrl->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN);
	mSetUrl->setText("Load url");

	// NOTE: playing video from local store is available only on Android.
	mSetPath = new Button();
	mSetPath->setFontColor(INTENSE_BLUE);
	// Apply a gradient on Android.
	mSetPath->setBackgroundGradient(SEA_GREEN, DARK_SEA_GREEN);
	mSetPath->setText("Load path");

	HorizontalLayout* urlLayout = new HorizontalLayout();
	urlLayout->setWidth(getScreenWidth()/2);
	urlLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	urlLayout->addChild(mSetUrl);

	HorizontalLayout* pathLayout = new HorizontalLayout();
	pathLayout->setWidth(getScreenWidth()/2);
	pathLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	pathLayout->addChild(mSetPath);

	mLoadLayout->addChild(urlLayout);
	mLoadLayout->addChild(pathLayout);

	mMainLayout->addChild(mLoadLayout);

	// Add an edit box for another urls or local paths.
	mEditBox = new EditBox();
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

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
	mVideoView->setHeight(getScreenHeight()/3);
	mVideoView->setWidth(getScreenWidth());
	mMainLayout->addChild(mVideoView);

	// Add duration label.
	mDuration = new Label();
	mDuration->setFontColor(INTENSE_BLUE);
	mDuration->setText("Video Duration");
	mMainLayout->addChild(mDuration);

	// Add a spacer of 10px before the buttons.
	mTopSpacerLayout = new VerticalLayout();
	mTopSpacerLayout->setBackgroundColor(INTENSE_BLUE);
	mTopSpacerLayout->setHeight(10);
	mMainLayout->addChild(mTopSpacerLayout);

	createPlaybackButtons();
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
		mDuration->setText("Video Duration");
        mVideoView->setURL(mEditBox->getText());
    }
    else if (button == mSetPath)
    {
		mDuration->setText("Video Duration");
		mVideoView->setPath(mEditBox->getText());
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
 * NOTE: there is a slightly different behaviour depending on the platform:
 *  - on iOS the source is loaded into memory when
 *  MAW_VIDEO_VIEW_STATE_PLAYING is received.
 *   - on Android the source is loaded into memory when
 *  MAW_VIDEO_VIEW_STATE_SOURCE_READY is received.
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
                mPause->setEnabled(true);
                mStop->setEnabled(true);
                // On iOS the source is loaded into memory and the duration
                // can be retrieved.
                if ( mVideoView->getDuration() > 0 )
                {
                mDuration->setText(
					"Duration: " +
					MAUtil::integerToString(mVideoView->getDuration()));
                }
                break;
            case MAW_VIDEO_VIEW_STATE_PAUSED:
                text = SOURCE_PAUSED;
                break;
            case MAW_VIDEO_VIEW_STATE_STOPPED:
                text = SOURCE_STOPPED;
                break;
            case MAW_VIDEO_VIEW_STATE_SOURCE_READY:
                text = SOURCE_READY;
                // Now pause and stop can be enabled.
                mPause->setEnabled(true);
                mStop->setEnabled(true);
                // On Android the source is loaded into memory at this point,
                // and the duration can be retrieved.
                if ( mVideoView->getDuration() > 0 )
                {
                mDuration->setText(
					"Duration: " +
					MAUtil::integerToString(mVideoView->getDuration()));
                }
                break;
            case MAW_VIDEO_VIEW_STATE_FINISHED:
                text = SOURCE_FINISHED;
                break;
            case MAW_VIDEO_VIEW_STATE_INTERRUPTED:
                text = SOURCE_ERROR;
                // Disable the control buttons.
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

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void VideoScreen::editBoxReturn(EditBox* editBox)
{
	// Make sure the virtual keyboard is closed when we hit return.
	mEditBox->hideKeyboard();
}
