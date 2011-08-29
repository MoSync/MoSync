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
 * @file NativeScreen.h
 * The main screen of the application.
 *
 * @author Emma Tresanszki
 */

#ifndef NATIVESCREEN_H_
#define NATIVESCREEN_H_


#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class NativeScreen:
	public Screen,
	public ButtonListener,
	public VideoViewListener
{

public:
	/**
	 * Constructor.
	 */
	NativeScreen();

	/**
	 * Destructor.
	 */
	~NativeScreen();

private:

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

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Creates and adds widgets to exemplify video widget.
	 */
	void addVideoWidgets();

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	VerticalLayout * mSpacerLayout;

	/** The video view widget. */
	VideoView* mVideoView;

	Label* mSourceStatus;

	HorizontalLayout* mButtonsLayout;
	Button* mPlay;
	Button* mPause;
	Button* mStop;

	/** Horizontal layout for seek to button and duration label. */
	HorizontalLayout* mSeekToLayout;

	/** Button for seeking to 100 millisecond. */
	Button* mSeekTo;

	/** Label that displays video total duration. */
	Label* mDuration;

	/** Edit box for setting the uri or local path. */
	EditBox* mEditBox;

	/** Horizontal layout for load url, load path buttons. */
	HorizontalLayout* mLoadLayout;
	Button* mSetUrl;
	Button* mSetPath;

	VerticalLayout* mSpacerBottomLayout;

	HorizontalLayout* mBottomLayout;
	Button* mExitButton;
};

#endif /* NATIVESCREEN_H_ */
