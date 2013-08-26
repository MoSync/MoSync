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
 * @file ImageScreen.h
 * @author Ali Sarrafi
 *
 * This application provides a very basic example of how to work
 * with Native UI and the Camera API
 */

// Include MoSync syscalls.
#include <maapi.h>

// Include NativeUI class interface
#include <NativeUI/Widgets.h>

#ifndef IMAGESCREEN_H_
#define IMAGESCREEN_H_

using namespace NativeUI;

/**
 * A class that encapsulates the behavior of ImageScreen
 */
class ImageScreen : public ButtonListener
{
public:
	ImageScreen()
	{
		mImageHandle = 0;
	}

	virtual ~ImageScreen()
	{
		delete mScreen;
	}

	virtual void buttonClicked(Widget* button);

	void initialize(StackScreen* stackScreen);

	void pushImageScreen();

	void setImageDataHandle(MAHandle dataHandle);

	void setImageDataRaw(int* data, int width, int height);

private:

	void createUI();

	Screen *mScreen;

	Button *mOKButton;

	VerticalLayout *mMainLayoutWidget;

	StackScreen *mStackScreen;

	Image *mImageWidget;

	MAHandle mImageHandle;

};

#endif /* IMAGESCREEN_H_ */
