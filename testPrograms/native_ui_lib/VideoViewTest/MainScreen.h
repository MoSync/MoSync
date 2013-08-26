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
 * @file MainScreen.h
 * @author Bogdan Iusco
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public ButtonListener,
	public VideoViewListener,
	public EditBoxListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

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
	 * Detects if the current platform is Android.
	 * @return true if the platform is Android, false otherwise.
	 */
	bool isAndroid();

	/**
	 * Detects if the current platform is Windows Phone.
	 * @return true if the platform is Windows Phone, false otherwise.
	 */
	bool isWindowsPhone();

	/**
	 * Handle the click event for video control button.
	 * Show/hide the video control and change button's text.
	 */
	void handleVideoControlButtonClicked();

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	VideoView* mVideoView;
	EditBox* mEditBox;

	Button* mSetUrl;
	Button* mPlay;
	Button* mPause;
	Button* mStop;
	Button* mGetDuration;
	Label* mDuration;
	Button* mSeekTo;
	Button* mCurrentTime;
	Label*  mTime;

	/**
	 * Show/hide video control.
	 */
	Button* mVideoControl;
};


#endif /* MAINSCREEN_H_ */
