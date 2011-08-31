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
 * @file VideoScreen.h
 * @author Emma Tresanszki
 *
 * The first screen of the application.
 * It contains widget controlls for Video View: Play, Pause, Stop,
 * an edit box for user input with the source link/path for the video.
 */

#ifndef NATIVESCREEN_H_
#define NATIVESCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class VideoScreen:
	public Screen,
	public ButtonListener,
	public VideoViewListener
{

public:
	/**
	 * Constructor.
	 */
	VideoScreen();

	/**
	 * Destructor.
	 */
	~VideoScreen();

	/**
	 * Load the default url. This can be set from SettingsScreen.
	 */
	void loadDefaultUrl(const MAUtil::String url);

	/**
	 * Show or hide the duration label.
	 * @param state if true show it, hide it otherwise.
	 */
	void showDurationLabel(bool state);

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Creates and adds widgets to exemplify video widget.
	 */
	void addVideoWidgets();

	/**
	 * Create the controls area with Play, Pause and Stop buttons.
	 */
	void createPlaybackButtons();

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button) {};

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button) {};

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

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
    virtual void videoViewStateChanged(
        VideoView* videoView,
        const int videoViewState);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/*
	 * An empty layout as a spacer.
	 */
	VerticalLayout * mTopSpacerLayout;

	/**
	 * The video view widget.
	 */
	VideoView* mVideoView;

	/**
	 * Label that notifies the user of the video playback status.
	 */
	Label* mSourceStatus;

	/**
	 * Horizontal layout with buttons for controlling playback.
	 */
	HorizontalLayout* mButtonsLayout;
	Button* mPlay;
	Button* mPause;
	Button* mStop;

	/**
	 * Label that displays video total duration.
	 */
	Label* mDuration;

	/**
	 *  Edit box for setting the uri or local path.
	 */
	EditBox* mEditBox;

	/**
	 * Horizontal layout for load url, load path buttons.
	 */
	HorizontalLayout* mLoadLayout;
	Button* mSetUrl;
	Button* mSetPath;

	VerticalLayout* mMiddleSpacerLayout;
	VerticalLayout* mSpacerBottomLayout;

	/**
	 * Exit button.
	 */
	Button* mExitButton;

	/**
	 * The screen width.
	 */
	int mScreenWidth;

	/**
	 * The screen height.
	 */
	int mScreenHeight;
};

#endif /* NATIVESCREEN_H_ */
