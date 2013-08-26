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
