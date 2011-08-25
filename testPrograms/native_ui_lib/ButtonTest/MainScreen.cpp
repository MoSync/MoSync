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

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mButton(NULL)
{
	createMainLayout();
	mButton->addEventListener(this);
	mButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mButton->removeEventListener(this);
	mButton->removeButtonListener(this);
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
	Button* testFontLabel = new Button();
	testFontLabel->setText("Test for this font!");
	testFontLabel->setFontColor(0xFF0000);
	testFontLabel->setFont(fontHandle);
	mMainLayout->addChild(testFontLabel);
}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonPressed(Widget* button)
{
    if (button == mButton)
    {
        printf("mButton pointerPressedEvent");
    }
}

/**
 * This method is called when there is an touch-up event for
 * a button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonReleased(Widget* button)
{
    if (button == mButton)
    {
        printf("mButton pointerReleasedEvent");
    }
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mButton)
    {
        printf("mButton buttonClickedEvent");
    }
}
