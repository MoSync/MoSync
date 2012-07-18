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
 * @file MainScreen.h
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Screen used to display logs.
 */

#ifndef PURCHASE_MAIN_SCREEN_H_
#define PURCHASE_MAIN_SCREEN_H_

#include <NativeUI/Widgets.h>
#include <MAUtil/String.h>

using namespace NativeUI;

namespace PurchaseTest
{

	class MainScreen: public Screen
	{
	public:
		/**
		 * Constructor.
		 */
		MainScreen();

		/**
		 * Destructor.
		 */
		virtual ~MainScreen();

		/**
		 * Write text on the screen.
		 * A new label containing the given text will be added to the screen.
		 * @param text Text to print.
		 */
		void printText(const MAUtil::String& text);

	private:
		/**
		 * Write text on the screen for iOS platform.
		 * A new label containing the given text will be added to the screen.
		 * @param text Text to print.
		 */
		void printTextiOS(const MAUtil::String& text);

		/**
		 * Write text on the screen for Android platform.
		 * A new list item containing the given text will be added to the list.
		 * @param text Text to print.
		 */
		void printTextAndroid(const MAUtil::String& text);

	private:
		/**
		 * Main layout.
		 * All widgets will be added to it.
		 */
		RelativeLayout* mMainLayout;
		ListView* mListView;

		/**
		 * Y coordinate for the next label that will be added to layout.
		 */
		int mLabelCoordY;
	};

} // namespace PurchaseTest


#endif /* PURCHASE_MAIN_SCREEN_H_ */
