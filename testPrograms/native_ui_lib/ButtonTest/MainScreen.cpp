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
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>

#include "MainScreen.h"

#define HORIZONTAL_LAYOUT_HEIGHT 80

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mButton(NULL),
	mEvents(NULL)
{
	createMainLayout();
	mButton->addEventListener(this);
	mButton->addButtonListener(this);
	mTestFontLabel->addButtonListener(this);
	mTestEnabledButton->addButtonListener(this);
	mCheckEnabledButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mButton->removeEventListener(this);
	mButton->removeButtonListener(this);
	mTestFontLabel->removeButtonListener(this);
	mTestEnabledButton->removeButtonListener(this);
	mCheckEnabledButton->removeButtonListener(this);
}

/**
 * This method is called when there is an event for this widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
void MainScreen::handleWidgetEvent(Widget* widget,
		MAWidgetEventData* widgetEventData)
{
	if (widget == mButton)
	{
		printf("button event");
	}
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	mButton = new Button();
	mMainLayout->addChild(mButton);

	int result = mButton->setTextVerticalAlignment(MAW_ALIGNMENT_BOTTOM);
	printf("result setTextVerticalAlignment = %d", result);

	result = mButton->setTextHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	printf("result setTextHorizontalAlignment = %d", result);

	result = mButton->setFontColor(0xFF0000);
	printf("result setFontColor = %d", result);

    result = mButton->setFontSize(15.9);
    printf("result setFontSize = %d", result);

	// Print number of available fonts.
	int fontsCount = maFontGetCount();
	Label* nrFontsLabel = new Label();

	nrFontsLabel->setText(MAUtil::integerToString(fontsCount) + " fonts available.");
	nrFontsLabel->setFontColor(0xFF0000);
	mMainLayout->addChild(nrFontsLabel);

	// Get the first font
	char buf[256];
	maFontGetName(0, buf, 256);
	Label* fontLoadedName = new Label();
	// Print the name of the font
	fontLoadedName->setText(buf);
	fontLoadedName->setFontColor(0xFF0000);
	mMainLayout->addChild(fontLoadedName);

	// Load the font with size 10 and get a handle.
	int fontHandle = maFontLoadWithName(buf, 10);

	// Set the handle to a label
	mTestFontLabel = new Button();
	mTestFontLabel->setText("Test for this font!");
	mTestFontLabel->setFontColor(0xFF0000);
	mTestFontLabel->setFont(fontHandle);
	mMainLayout->addChild(mTestFontLabel);

	// Create and add the 'isEnabled' test widgets
	mHorizontalLayout = new HorizontalLayout();
	mHorizontalLayout->fillSpaceHorizontally();
	mHorizontalLayout->setHeight(HORIZONTAL_LAYOUT_HEIGHT);

	mTestEnabledButton = new Button();
	mTestEnabledButton->fillSpaceHorizontally();
	mTestEnabledButton->setText("Enabled");
	mHorizontalLayout->addChild(mTestEnabledButton);

	mCheckEnabledButton = new Button();
	mCheckEnabledButton->fillSpaceHorizontally();
	mCheckEnabledButton->setText("<- Disable");
	mHorizontalLayout->addChild(mCheckEnabledButton);

	mMainLayout->addChild(mHorizontalLayout);

	mEvents = new ListView();
	mEvents->fillSpaceHorizontally();
	mEvents->fillSpaceVertically();
	mMainLayout->addChild(mEvents);
}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonPressed(Widget* button)
{
	Label* event = new Label();
    if (button == mButton)
    {
        printf("mButton pointerPressedEvent");
        event->setText("buttonPressed");
    }
    mEvents->addChild(event);
}

/**
 * This method is called when there is an touch-up event for
 * a button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonReleased(Widget* button)
{
    if (button == mButton)
    {
        printf("mButton pointerReleasedEvent");
    }
    Label* event = new Label();
    event->setText("buttonReleased");
    mEvents->addChild(event);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	Label* event = new Label();
	if (button == mButton)
	{
		printf("mButton buttonClickedEvent");
		event->setText("buttonClicked");
	}
	else if (button == mCheckEnabledButton)
	{
		if (mTestEnabledButton->isEnabled())
		{
			mTestEnabledButton->setEnabled(false);
			mTestEnabledButton->setText("Disabled");
			mCheckEnabledButton->setText("<- Enable");
			event->setText("Button was enabled -> button is now disabled");
		}
		else
		{
			mTestEnabledButton->setEnabled(true);
			mTestEnabledButton->setText("Enabled");
			mCheckEnabledButton->setText("<- Disable");
			event->setText("Button was disabled -> button is now enabled");
		}
	}
	mEvents->addChild(event);
}
