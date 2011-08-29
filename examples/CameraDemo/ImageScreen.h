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
 * @file ImageScreen.h
 * @author Ali Sarrafi
 *
 * This application provides a very basic example of how to work
 * with Native UI and the Camera API
 */

// Include MoSync syscalls.
#include <maapi.h>
#include <IX_WIDGET.h>
#ifndef IMAGESCREEN_H_
#define IMAGESCREEN_H_


/**
 * A class that encapsulates the behavior of ImageScreen
 */
class ImageScreen
{
public:
	ImageScreen():isViewed(false)
	{
		//do nothing
	}

	virtual ~ImageScreen()
	{
		maWidgetDestroy(mScreen);
	}

	void customEvent(const MAEvent& event);

	int initialize(MAHandle stackScreen);

	void pushImageScreen();

	void setImageDataHandle(MAHandle dataHandle);

	bool isViewed;

private:

	void createUI();

	MAHandle mScreen;

	MAHandle mOKButton;

	MAHandle mMainLayoutWidget;

	MAHandle mStackScreen;

	MAHandle mImageWidget;

	MAHandle dataHandle;

	MAHandle imageHandle;
};

#endif /* IMAGESCREEN_H_ */
