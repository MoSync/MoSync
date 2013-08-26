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
 * @file LoadingScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show an ActivityIndicatior.
 * It does not interact with the user, used only when the application
 * is reading data from files.
 */

#define LOADING_LABEL_TEXT "Loading..."

#include <NativeUI/ActivityIndicator.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>

#include "LoadingScreen.h"
#include "ViewUtils.h"

namespace EuropeanCountries
{

	/**
	 * Constructor.
	 */
	LoadingScreen::LoadingScreen():
		mMainLayout(NULL)
	{
		this->createUI();
	}

	/**
	 * Create screen's UI.
	 */
	void LoadingScreen::createUI()
	{
		// Create and add main layout to the screen.
		mMainLayout = new NativeUI::VerticalLayout();
		this->setMainWidget(mMainLayout);

		// Add spacer.
		mMainLayout->addChild(new NativeUI::VerticalLayout);

		// Add ActivityIndicatior.
		NativeUI::ActivityIndicator* activityIndicator =
			new NativeUI::ActivityIndicator();
		mMainLayout->addChild(activityIndicator);

		if (isWindowsPhone())
		{
			// For WP7 platform add an Label widget.
			NativeUI::Label* loading = new NativeUI::Label(LOADING_LABEL_TEXT);
			loading->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			loading->fillSpaceHorizontally();
			mMainLayout->addChild(loading);

			mMainLayout->addChild(createSpacer(SPACER_HEIGHT));
			activityIndicator->fillSpaceHorizontally();
		}
		else
		{
			mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		}

		// Add spacer.
		mMainLayout->addChild(new NativeUI::VerticalLayout);

		// Show the ActivityIndicator.
		activityIndicator->show();
	}

} // end of EuropeanCoutnries
