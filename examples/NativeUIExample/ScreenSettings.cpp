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
 * @file ScreenSettings.h
 * @author TODO: Add your name.
 *
 * TODO: Add comment.
 */

#include <mavsprintf.h>
#include "MAHeaders.h"
#include "ScreenSettings.h"

/**
 * Listener for button click events.
 */
class ButtonEventListener : public WidgetEventListener
{
public:
	ButtonEventListener() :
		mButtonClickCount(0)
	{
	}

	/**
	 * This method is called when there is an event for this widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	void handleWidgetEvent(
		Widget* widget,
		MAWidgetEventData* widgetEventData)
	{
		// Is this a click event?
		if (MAW_EVENT_CLICKED == widgetEventData->eventType)
		{
			++mButtonClickCount;
			char buffer[256];
			if (1 == mButtonClickCount)
			{
				sprintf(buffer, "You clicked me once!", mButtonClickCount);
			}
			else
			if (99 < mButtonClickCount)
			{
				sprintf(buffer, "Wow! %d times!", mButtonClickCount);
			}
			else
			{
				sprintf(buffer, "You clicked me %d times!", mButtonClickCount);
			}
			((Button*)widget)->setText(buffer);
		}
	}

private:
	int mButtonClickCount;
};

/**
 * TODO: Add comment.
 */
Screen* ScreenSettings::create()
{
	// Create screen.
	Screen* screen = new Screen();
	screen->setTitle("Settings");
	screen->setIcon(RES_TAB_ICON_SETTINGS);

	// Create a layout.
	VerticalLayout* layout = new VerticalLayout();
	layout->setBackgroundColor(255, 255, 255);

	// Create a button.
	Button* widget = new Button();
	widget->setText("TODO: Implement a settings screen");
	layout->addChild(widget);

	// Create another clickable button.
	Button* button = new Button();
	button->setText("Click me!");
	button->setFontColor(0, 0, 0);
	button->setFontSize(36);
	button->setEventListener(new ButtonEventListener());
	layout->addChild(button);

	screen->setMainWidget(layout);

	return screen;
}
