/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
