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
 * @file MainScreen.h
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Class that creates a screen that displays all the contacts.
 */
class MainScreen:
	public Screen,
	public WidgetEventListener
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
	 * This method is called when there is an event for this widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	virtual void handleWidgetEvent(
		Widget* widget,
		MAWidgetEventData* widgetEventData);

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	Image* mImage;
};


#endif /* MAINSCREEN_H_ */
