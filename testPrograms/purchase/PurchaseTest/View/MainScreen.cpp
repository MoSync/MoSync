/*
 Copyright (C) 2012 MoSync AB

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
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Screen used to display logs.
 */

// Default size for labels
#define LABEL_HEIGHT 35

#include <conprint.h>

#include "MainScreen.h"
#include "../Util.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 */
	MainScreen::MainScreen():
		Screen(),
		mMainLayout(NULL),
		mLabelCoordY(0)
	{
		int platform = getPlatform();
		if (platform == ANDROID)
		{
			mListView = new ListView();
			this->setMainWidget(mListView);
		}
		else if (platform == IOS)
		{
			mMainLayout = new RelativeLayout();
			mMainLayout->setScrollable(true);
			this->setMainWidget(mMainLayout);
		}
	}

	/**
	 * Destructor.
	 */
	MainScreen::~MainScreen()
	{

	}

	/**
	 * Write text on the screen.
	 * A new label containing the give text will be added to the screen.
	 * @param text Text to print.
	 */
	void MainScreen::printText(const MAUtil::String& text)
	{
		printf("LOG: %s", text.c_str());
		int platform = getPlatform();
		if (platform == ANDROID)
		{
			this->printTextAndroid(text);
		}
		else if (platform == IOS)
		{
			this->printTextiOS(text);
		}
	}

	/**
	 * Write text on the screen for iOS platform.
	 * A new label containing the given text will be added to the screen.
	 * @param text Text to print.
	 */
	void MainScreen::printTextiOS(const MAUtil::String& text)
	{
		Label* label = new Label();
		label->setLeftPosition(0);
		label->setTopPosition(mLabelCoordY);
		label->setText(text);

		// The label must be have a fix height.
		label->setHeight(LABEL_HEIGHT);
		mMainLayout->addChild(label);

		mLabelCoordY += LABEL_HEIGHT;

		int scrollToY = mLabelCoordY - this->getHeight();
		if (scrollToY > 0)
		{
			mMainLayout->setContentOffset(0, scrollToY);
		}
	}

	/**
	 * Write text on the screen for Android platform.
	 * A new list item containing the given text will be added to the list.
	 * @param text Text to print.
	 */
	void MainScreen::printTextAndroid(const MAUtil::String& text)
	{
		ListViewItem* item = new ListViewItem();
		item->setText(text);
		mListView->addChild(item);
	}
}
