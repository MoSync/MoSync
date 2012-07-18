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
 * @author Bogdan Iusco and Emma Tresanszki.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"

int count_clicks;

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mLabel(NULL),
	nrFontsLabel(NULL),
	fontLoadedName(NULL),
	testFontLabel(NULL)
{
	createMainLayout();

}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	count_clicks = 0;

	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x9BCD9B);
	Screen::setMainWidget(mMainLayout);

	mLabel = new Label();
	mMainLayout->addChild(mLabel);

	mLabel->setText("this is a long label!this is a long label!this is a long label!this is a long label!this is a long label!this is a long label!this is a long label!this is a long label!this is a long label!this is a long label!");
	mLabel->setFontColor(0xFF0000);
	mLabel->setMaxNumberOfLines(10);
	mLabel->fillSpaceHorizontally();

	mLabel->setHeight(200);
	mLabel->setTextVerticalAlignment(MAW_ALIGNMENT_TOP);

	int result = mLabel->getMaxNumberOfLines();
	printf("result getMaxNumberOfLines = %d", result);

	mButton = new Button();
	mButton->setText("Vertical alignment: Top");
	mButton->fillSpaceHorizontally();
	mButton->addButtonListener(this);
	mMainLayout->addChild(mButton);


	// Print number of available fonts.
	int fontsCount = maFontGetCount();
	nrFontsLabel = new Label();

	nrFontsLabel->setText(MAUtil::integerToString(fontsCount) + " fonts available.");
	nrFontsLabel->setFontColor(0xFF0000);
	mMainLayout->addChild(nrFontsLabel);

	// Get the first font
	char buf[256];
	maFontGetName(0, buf, 256);
	fontLoadedName = new Label();
	// Print the name of the font
	fontLoadedName->setText(buf);
	fontLoadedName->setFontColor(0xFF0000);
	mMainLayout->addChild(fontLoadedName);

	// Load the font with size 10 and get a handle.
	int fontHandle = maFontLoadWithName(buf, 10);

	// Set the handle to a label
	testFontLabel = new Label();
	testFontLabel->setText("Test for this font!");
	testFontLabel->setFontColor(0xFF0000);
	testFontLabel->setFont(fontHandle);
	mMainLayout->addChild(testFontLabel);
}

void MainScreen::buttonClicked(Widget* button)
{
	if(button == mButton)
	{
		switch(count_clicks % 3)
		{
		case 0:
			mLabel->setTextVerticalAlignment(MAW_ALIGNMENT_BOTTOM);
			mButton->setText("Vertical alignment: Bottom");
			count_clicks = count_clicks % 3;
			break;
		case 1:
			mLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
			mButton->setText("Vertical alignment: Center");
			count_clicks = count_clicks % 3;
			break;
		case 2:
			mLabel->setTextVerticalAlignment(MAW_ALIGNMENT_TOP);
			mButton->setText("Vertical alignment: Top");
			count_clicks = count_clicks % 3;
			break;
		}
		count_clicks++;
	}
}
