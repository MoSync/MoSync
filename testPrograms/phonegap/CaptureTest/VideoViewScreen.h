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
 * @file VideoViewScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a recorded video.
 */

#ifndef VIDEOVIEWSCREEN_H_
#define VIDEOVIEWSCREEN_H_


// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <maapi.h>

using namespace NativeUI;
using namespace MAUtil;

/**
 * @brief Screen used for displaying a recorded video.
 */
class VideoViewScreen:
	public Screen,
	public ButtonListener
{
public:
	/**
	 * Constructor.
	 */
		VideoViewScreen();

	/**
	 * Destructor.
	 */
	virtual ~VideoViewScreen();

	/**
	 * Set the video file path.
	 * @param path Path to the video file.
	 */
	void setVideoFilePath(const MAUtil::String& path);

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

private:
	/**
	 * Used for playing the video file.
	 */
	VideoView* mVideoView;

	/**
	 * Used for starting the video.
	 */
	Button* mPlayBtn;

	/**
	 * Used for pausing the video.
	 */
	Button* mPauseBtn;
};


#endif /* VIDEOVIEWSCREEN_H_ */
