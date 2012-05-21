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
		mMainLayout = new RelativeLayout();
		//mMainLayout->setScrollable(true);
		mListView = new ListView();
		mMainLayout->addChild(mListView);
		this->setMainWidget(mMainLayout);
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
		Label* label = new Label();
		//label->setLeftPosition(0);
		//label->setTopPosition(mLabelCoordY);
		label->setText(text);

		// The label must be have a fix height.
//		label->setHeight(LABEL_HEIGHT);
		//mMainLayout->addChild(label);
		mListView->addChild(label);

		mLabelCoordY += LABEL_HEIGHT;

		int scrollToY = mLabelCoordY - this->getHeight();
		if (scrollToY > 0)
		{
			printf("scroll to y: %d", scrollToY);
			mMainLayout->setContentOffset(0,scrollToY);
			printf("layout content offset (x,y) = (%d, %d)",
				mMainLayout->getContentOffsetCoordX(),
				mMainLayout->getContentOffsetCoordY());
		}
	}
}
