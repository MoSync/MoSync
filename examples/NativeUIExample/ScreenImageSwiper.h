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
 * @file ScreenImageSwiper.h
 * @author TODO: Add your name.
 *
 * TODO: Add comment.
 */

#ifndef SCREEN_IMAGE_SWIPER_H_
#define SCREEN_IMAGE_SWIPER_H_

#include "UIWrapper/Widgets.h"
#include "MAHeaders.h"

#include "conprint.h"

/*
 * DEFINES
 */
#define LEFT_IMAGE				0
#define CENTER_IMAGE			1
#define RIGHT_IMAGE				2
#define DISPLAYED_IMAGES		3

#define SCREEN_WIDTH 			320

/*
 * DEFINES
 */


using namespace MoSync::UI;

/**
 * TODO: Add comment.
 */
class ScreenImageSwiper : public WidgetEventListener
{
private:
	int mScreenWidth;
	int mScreenHeight;
public:
	/**
	 * TODO: Add comment.
	 */
	static Screen* create();

	static void destroy();

	/**
	 * This method is called when there is an event for this widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	void handleWidgetEvent(
		Widget* widget,
		MAWidgetEventData* widgetEventData);

private:
	void getScreenSize();
};

#endif
